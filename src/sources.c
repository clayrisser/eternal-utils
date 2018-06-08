#include <glib.h>
#include "glib-extras.h"
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
  GError* err;
  gchar* line;
  sources = NULL;
  err = NULL;
  line = strtok(content, "\n");
  while (line) {
    GRegex* regex;
    gchar* regex_str;
    GMatchInfo* match_info;
    regex_str = "([ \t]*#[ \t]*)?(source[ \t]+)([^\n]+)";
    regex = g_regex_new(regex_str, 0, 0, &err);
    g_regex_match(regex, line, 0, &match_info);
    while (g_match_info_matches(match_info)) {
      gchar** matches;
      matches = g_match_info_fetch_all(match_info);
      if (!matches[1] || strlen(matches[1]) <= 0) {
        gchar* source;
        source = trim(matches[3]);
        sources = g_list_append(sources, source);
      }
      g_match_info_next(match_info, NULL);
    }
    g_match_info_unref(match_info);
    g_regex_unref(regex);
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
    gboolean duplicate = FALSE;
    source = l->data;
    GList* l2;
    for (l2 = l->next; l2 != NULL; l2 = l2->next) {
      if (g_strcmp0(expand_path(source), expand_path(l2->data)) == 0) {
        duplicate = TRUE;
      }
    }
    g_list_free(l2);
    if (!duplicate) {
      content = g_strconcat(content, "source ", "\"", source, "\"\n", NULL);
    }
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
  sources_path = g_canonicalize_filename(sources_filename, g_get_home_dir());
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
  for (gint i = 1; i < argc; i++) {
    gchar* arg;
    arg = argv[i];
    GList* l;
    for (l = sources; l != NULL; l = l->next) {
      gchar* source;
      source = l->data;
      if (g_strcmp0(expand_path(arg), expand_path(source)) == 0) {
        sources = g_list_remove_all(sources, source);
      }
    }
    g_list_free(l);
  }
  return sources;
}
