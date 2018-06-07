#include <pcre.h>
#include <glib.h>
#include "shared.h"

GHashTable* get_aliases_from_content(char* content) {
  GHashTable* aliases;
  aliases = g_hash_table_new(g_str_hash, g_str_equal);
  char* line;
  line = strtok(content, "\n");
  while(line) {
    GList* matches;
    matches = regex("([ \t]*#[ \t]*)?(alias[ \t]+)?([0-9a-zA-Z_]+)[ \t]*=[ \t]*([^\n]*)",
                    line, PCRE_MULTILINE||PCRE_EXTENDED);
    if (g_list_length(matches) >= 5) {
      if (strlen(g_list_nth(matches, 1)->data) <= 0) {
        gchar* key;
        gchar* value;
        key = g_list_nth(matches, 3)->data;
        value = trim(g_list_nth(matches, 4)->data);
        g_hash_table_insert(aliases, key, value);
      }
    }
    g_list_free(matches);
    line = strtok(NULL, "\n");
  }
  return aliases;
}

gchar* get_content_from_aliases(GHashTable* aliases) {
  gchar* content;
  content = "#!/bin/bash\n\n";
  GList* keys = g_hash_table_get_keys(aliases);
  GList* l;
  for (l = keys; l != NULL; l = l->next) {
    gchar* key;
    gchar* value;
    key = l->data;
    value = g_hash_table_lookup(aliases, key);
    content = g_strconcat(content, "alias ", key, "=\"", value, "\"\n", NULL);
  }
  g_list_free(l);
  g_list_free(keys);
  return content;
}

gchar* get_aliases_path() {
  gchar* aliases_filename;
  gchar* aliases_path;
  gchar* shell;
  shell = get_shell();
  aliases_filename = g_strconcat(".", shell, "_aliases", NULL);
  aliases_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), aliases_filename, NULL);
  return aliases_path;
}

gboolean* write_aliases(GHashTable* aliases) {
  gboolean result;
  gchar* content;
  gchar* aliases_path;
  aliases_path = get_aliases_path();
  content = get_content_from_aliases(aliases);
  result = write_file(aliases_path, content);
  return result;
}

GHashTable* get_aliases_from_args(gint argc, gchar* argv[], GHashTable* aliases) {
  gchar* command;
  command = "alias";
  for (int i = 1; i < argc; i++) {
    const gchar delim[2] = "=";
    gchar* key;
    gchar* token;
    gchar* value;
    gchar* arg = malloc(strlen(argv[i])+1);
    strcpy(arg, argv[i]);
    token = strtok(arg, delim);
    value = "";
    for(int i = 0; i < 2; i++) {
      if (token != NULL) {
        if (i == 0) {
          key = token;
        } else if (i == 1) {
          value = token;
        } else {
          break;
        }
        token = strtok(NULL, delim);
      } else {
        break;
      }
    }
    if (strlen(key) > 0) {
      g_hash_table_insert(aliases, key, value);
    }
    command = g_strconcat(command, " \"", argv[i], "\"", NULL);
  }
  command = g_strconcat(command, " 1>/dev/null", NULL);
  int err = system(command);
  if (err) {
    exit(1);
  }
  return aliases;
}

GHashTable* get_eternal_aliases(gint argc, gchar* argv[]) {
  GHashTable* aliases;
  gchar* content;
  gchar* aliases_path;
  aliases_path = get_aliases_path();
  content = read_file(aliases_path);
  aliases = get_aliases_from_content(content);
  aliases = get_aliases_from_args(argc, argv, aliases);
  return aliases;
}
