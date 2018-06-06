#include <pcre.h>
#include <glib.h>
#include "shared.h"

gchar* gen_key(GHashTable* envs, gchar* key) {
  if (g_hash_table_lookup(envs, key) != NULL) {
    GList* matches;
    gint count;
    count = 0;
    matches = regex("([0-9a-zA-Z_]+)(-[0-9]+)?", key, 0);
    if (g_list_length(matches) >= 2) {
      gchar* body;
      gchar* tail;
      if (g_list_length(matches) >= 3) {
        body = g_list_nth(matches, 1)->data;
        tail = g_list_nth(matches, 2)->data;
        if (strlen(tail) > 0) {
          count = atoi(tail + 1) + 1;
        }
      } else {
        body = key;
      }
      key = g_strconcat(body, "-", g_strdup_printf("%i", count));
      key = gen_key(envs, key);
    }
  }
  return key;
}

GHashTable* get_envs_from_content(char* content) {
  GHashTable* envs;
  envs = g_hash_table_new(g_str_hash, g_str_equal);
  char* line;
  line = strtok(content, "\n");
  while(line) {
    GList* matches;
    matches = regex("([ \t]*#[ \t]*)?(export[ \t]+)?([0-9a-zA-Z_]+)[ \t]*=[ \t]*([^\n]*)",
                    line, PCRE_MULTILINE||PCRE_EXTENDED);
    if (g_list_length(matches) >= 5) {
      if (strlen(g_list_nth(matches, 1)->data) <= 0) {
        gchar* key;
        gchar* value;
        key = g_list_nth(matches, 3)->data;
        key = gen_key(envs, key);
        value = g_list_nth(matches, 4)->data;
        g_hash_table_insert(envs, key, value);
      }
    }
    line = strtok(NULL, "\n");
  }
  return envs;
}

gchar* get_envs_path() {
  gchar* envs_filename;
  gchar* envs_path;
  gchar* shell;
  shell = get_shell();
  envs_filename = g_strconcat(".", shell, "_envs", NULL);
  envs_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), envs_filename, NULL);
  g_free(shell);
  return envs_path;
}

GHashTable* get_eternal_envs() {
  gchar* content;
  gchar* envs_path;
  GHashTable* envs;
  envs_path = get_envs_path();
  content = read_file(envs_path);
  envs = get_envs_from_content(content);
  g_free(envs_path);
  g_free(content);
  return envs;
}
