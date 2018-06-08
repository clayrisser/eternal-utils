#include <glib.h>
#include "shared.h"

gboolean aliases_sourced() {
  gboolean sourced;
  gchar* shell;
  shell = get_shell();
  sourced = FALSE;
  if (is_sourced(g_strconcat("[$]HOME\/\.", shell, "_aliases", NULL))) {
    sourced = TRUE;
  }
  if (is_sourced(g_strconcat("~\/\.", shell, "_aliases", NULL))) {
    sourced = TRUE;
  }
  return sourced;
}

GHashTable* get_aliases_from_content(gchar* content) {
  GError* err;
  GHashTable* aliases;
  gchar* line;
  aliases = g_hash_table_new(g_str_hash, g_str_equal);
  err = NULL;
  line = strtok(content, "\n");
  while (line) {
    GRegex* regex;
    gchar* regex_str;
    GMatchInfo* match_info;
    regex_str = "([ \t]*#[ \t]*)?(alias[ \t]+)([^ ]+)[ \t]*=[ \t]*([^\n]*)";
    regex = g_regex_new(regex_str, 0, 0, &err);
    g_regex_match(regex, line, 0, &match_info);
    while (g_match_info_matches(match_info)) {
      gchar** matches;
      matches = g_match_info_fetch_all(match_info);
      if (strlen(matches[1]) <= 0) {
        gchar* key;
        gchar* value;
        key = matches[3];
        value = trim(matches[4]);
        g_hash_table_insert(aliases, key, value);
      }
      g_match_info_next(match_info, NULL);
    }
    g_match_info_unref(match_info);
    g_regex_unref(regex);
    line = strtok(NULL, "\n");
  }
  return aliases;
}

gchar* get_content_from_aliases(GHashTable* aliases) {
  gchar* content;
  GList* keys;
  content = "#!/bin/bash\n\n";
  keys = g_hash_table_get_keys(aliases);
  keys = sort_list(keys);
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
  gchar* simlink_path;
  shell = get_shell();
  aliases_filename = g_strconcat(".", shell, "_aliases", NULL);
  aliases_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), aliases_filename, NULL);
  simlink_path = g_file_read_link(aliases_path, NULL);
  if (simlink_path) {
    aliases_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), simlink_path, NULL);
  }
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
  for (gint i = 1; i < argc; i++) {
    const gchar delim[2] = "=";
    gchar* key;
    gchar* token;
    gchar* value;
    gchar* arg = malloc(strlen(argv[i])+1);
    strcpy(arg, argv[i]);
    token = strtok(arg, delim);
    value = "";
    for(gint i = 0; i < 2; i++) {
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
  gint err = system(command);
  if (err) {
    exit(1);
  }
  return aliases;
}

GHashTable* get_eternal_aliases() {
  GHashTable* aliases;
  gchar* content;
  gchar* aliases_path;
  if (!aliases_sourced()) {
    gchar* shell;
    gchar* shell_path;
    shell = get_shell();
    shell_path = get_shell_path();
    append_file(shell_path, g_strconcat("\nsource $HOME/.", shell, "_aliases\n", NULL));
  }
  aliases_path = get_aliases_path();
  content = read_file(aliases_path);
  aliases = get_aliases_from_content(content);
  return aliases;
}

GHashTable* unalias_eternal_aliases(gint argc, gchar* argv[], GHashTable* aliases) {
  for (gint i = 1; i < argc; i++) {
    g_hash_table_remove(aliases, argv[i]);
  }
  return aliases;
}
