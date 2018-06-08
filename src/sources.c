#include <pcre.h>
#include <glib.h>
#include "shared.h"

gboolean sources_sourced() {
  gboolean sourced;
  gchar* shell;
  shell = get_shell();
  sourced = FALSE;
  if (is_sourced(g_strconcat("[$]HOME\/\.", shell, "_sources", NULL))) {
    sourced = TRUE;
  }
  if (is_sourced(g_strconcat("~\/\.", shell, "_sources", NULL))) {
    sourced = TRUE;
  }
  return sourced;
}

GList* get_sources_from_content(gchar* content) {
  GList* sources;
  char* line;
  sources = NULL;
  line = strtok(content, "\n");
  while(line) {
    GList* matches;
    matches = regex("([ \t]*#[ \t]*)?(source[ \t]+)([^\n]+)",
                    line, PCRE_MULTILINE||PCRE_EXTENDED);
    if (g_list_length(matches) >= 4) {
      if (strlen(g_list_nth(matches, 1)->data) <= 0) {
        gchar* source;
        source = trim(g_list_nth(matches, 3)->data);
        sources = g_list_append(sources, source);
      }
    }
    g_list_free(matches);
    line = strtok(NULL, "\n");
  }
  return sources;
}

gchar* get_content_from_sources(GList* sources) {
  gchar* content;
  content = "#!/bin/bash\n\n";
  GList* l;
  for (l = sources; l != NULL; l = l->next) {
    gchar* source;
    source = l->data;
    content = g_strconcat(content, "source ", "\"", source, "\"\n", NULL);
  }
  g_list_free(l);
  return content;
}

gchar* get_sources_path() {
  gchar* sources_filename;
  gchar* sources_path;
  gchar* shell;
  gchar* simlink_path;
  shell = get_shell();
  sources_filename = g_strconcat(".", shell, "_sources", NULL);
  sources_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), sources_filename, NULL);
  simlink_path = g_file_read_link(sources_path, NULL);
  if (simlink_path) {
    sources_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), simlink_path, NULL);
  }
  return sources_path;
}

gboolean* write_sources(GList* sources) {
  gboolean result;
  gchar* content;
  gchar* sources_path;
  sources_path = get_sources_path();
  content = get_content_from_sources(sources);
  result = write_file(sources_path, content);
  return result;
}

GList* get_sources_from_args(gint argc, gchar* argv[], GList* sources) {
  gchar* command;
  if (argc >= 2) {
    sources = g_list_append(sources, argv[1]);
  }
  sources = uniq(sources);
  return sources;
}

GList* get_eternal_sources() {
  GList* sources;
  gchar* content;
  gchar* sources_path;
  if (!sources_sourced()) {
    gchar* shell;
    gchar* shell_path;
    shell = get_shell();
    shell_path = get_shell_path();
    append_file(shell_path, g_strconcat("\nsource $HOME/.", shell, "_sources\n", NULL));
  }
  sources_path = get_sources_path();
  content = read_file(sources_path);
  sources = get_sources_from_content(content);
  return sources;
}

GList* unsource_eternal_sources(gint argc, gchar* argv[], GList* sources) {
  gchar* command;
  for (int i = 1; i < argc; i++) {
    g_hash_table_remove(sources, argv[i]);
  }
  command = g_strconcat(command, " 1>/dev/null", NULL);
  int err = system(command);
  if (err) {
    exit(1);
  }
  return sources;
}
