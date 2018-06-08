#include <glib.h>
#include "glib-extras.h"
#include "shared.h"

gboolean envs_sourced() {
  gboolean sourced;
  gchar* shell;
  shell = get_shell();
  sourced = FALSE;
  if (is_sourced(g_strconcat("[$]HOME\/\.", shell, "_envs", NULL))) {
    sourced = TRUE;
  }
  if (is_sourced(g_strconcat("~\/\.", shell, "_envs", NULL))) {
    sourced = TRUE;
  }
  return sourced;
}

gchar* encode_key(GHashTable* envs, gchar* key) {
  GError* err;
  err = NULL;
  if (g_hash_table_lookup(envs, key) != NULL) {
    GMatchInfo* match_info;
    GRegex* regex;
    gchar* body;
    gchar* regex_str;
    gchar* tail;
    gchar** matches;
    gint count;
    count = 0;
    regex_str = "([0-9a-zA-Z_]+)(-[0-9]+)?";
    regex = g_regex_new(regex_str, 0, 0, &err);
    g_regex_match(regex, key, 0, &match_info);
    matches = g_match_info_fetch_all(match_info);
    body = matches[1];
    tail = matches[2];
    if (tail) {
      count = atoi(tail+1) + 1;
    } else {
      body = key;
    }
    key = g_strconcat(body, "-", g_strdup_printf("%i", count), NULL);
    key = encode_key(envs, key);
    g_match_info_unref(match_info);
    g_regex_unref(regex);
  }
  return key;
}

gchar* decode_key(gchar* key) {
  gchar* found;
  found = strchr(key, '-');
  if (found != NULL) {
    g_utf8_strncpy(key, key, strlen(key)-strlen(found));
  }
  return key;
}

GHashTable* get_envs_from_content(gchar* content) {
  GError* err;
  GHashTable* envs;
  gchar* line;
  envs = g_hash_table_new(g_str_hash, g_str_equal);
  err = NULL;
  line = strtok(content, "\n");
  while (line) {
    GRegex* regex;
    gchar* regex_str;
    GMatchInfo* match_info;
    regex_str = "([ \t]*\#[ \t]*)?(export[ \t]+)?([0-9a-zA-Z_]+)[ \t]*=[ \t]*([^\n]*)";
    regex = g_regex_new(regex_str, 0, 0, &err);
    g_regex_match(regex, line, 0, &match_info);
    while (g_match_info_matches(match_info)) {
      gchar** matches;
      matches = g_match_info_fetch_all(match_info);
      if (strlen(matches[1]) <= 0) {
        gchar* key;
        gchar* value;
        key = encode_key(envs, matches[3]);
        value = trim(matches[4]);
        g_hash_table_insert(envs, key, value);
      }
      g_match_info_next(match_info, NULL);
    }
    g_match_info_unref(match_info);
    g_regex_unref(regex);
    line = strtok(NULL, "\n");
  }
  return envs;
}

gchar* get_content_from_envs(GHashTable* envs) {
  gchar* content;
  GList* keys;
  content = "#!/bin/bash\n\n";
  keys = g_hash_table_get_keys(envs);
  GList* l;
  for (l = keys; l != NULL; l = l->next) {
    gchar* key;
    gchar* value;
    key = decode_key(l->data);
    value = g_hash_table_lookup(envs, key);
    content = g_strconcat(content, "export ", key, "=\"", value, "\"\n", NULL);
  }
  g_list_free(l);
  g_list_free(keys);
  return content;
}

gchar* get_envs_path() {
  gchar* envs_filename;
  gchar* envs_path;
  gchar* shell;
  gchar* simlink_path;
  shell = get_shell();
  envs_filename = g_strconcat(".", shell, "_envs", NULL);
  envs_path = g_canonicalize_filename(envs_filename, g_get_home_dir());
  simlink_path = g_file_read_link(envs_path, NULL);
  if (simlink_path) {
    envs_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), simlink_path, NULL);
  }
  return envs_path;
}

gboolean* write_envs(GHashTable* envs) {
  gboolean result;
  gchar* content;
  gchar* envs_path;
  envs_path = get_envs_path();
  content = get_content_from_envs(envs);
  result = write_file(envs_path, content);
  return result;
}

GHashTable* get_envs_from_args(gint argc, gchar* argv[], GHashTable* envs) {
  gchar* command;
  command = "export";
  for (gint i = 1; i < argc; i++) {
    const gchar delim[2] = "=";
    gchar* key;
    gchar* token;
    gchar* value;
    gchar* arg = g_malloc(strlen(argv[i])+1);
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
      g_hash_table_insert(envs, key, value);
    }
    command = g_strconcat(command, " \"", argv[i], "\"", NULL);
  }
  command = g_strconcat(command, " 1>/dev/null", NULL);
  gint err = system(command);
  if (err) {
    exit(1);
  }
  return envs;
}

GHashTable* get_eternal_envs() {
  GHashTable* envs;
  gchar* content;
  gchar* envs_path;
  if (!envs_sourced()) {
    gchar* shell;
    gchar* shell_path;
    shell = get_shell();
    shell_path = get_shell_path();
    append_file(shell_path, g_strconcat("\nsource $HOME/.", shell, "_envs\n", NULL));
  }
  envs_path = get_envs_path();
  content = read_file(envs_path);
  envs = get_envs_from_content(content);
  return envs;
}

GHashTable* unset_eternal_envs(gint argc, gchar* argv[], GHashTable* envs) {
  gchar* command;
  command = "unset";
  for (gint i = 1; i < argc; i++) {
    if (g_strcmp0(g_utf8_strup(argv[i], -1), "PATH")) {
      g_hash_table_remove(envs, argv[i]);
    }
    command = g_strconcat(command, " ", argv[i], NULL);
  }
  command = g_strconcat(command, " 1>/dev/null", NULL);
  gint err = system(command);
  if (err) {
    exit(1);
  }
  return envs;
}
