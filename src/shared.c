#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

gchar* get_shell() {
  GError* err;
  gchar* shell;
  err = NULL;
  shell = getenv("SHELL");
  if (!shell) {
    shell = "bash";
  } else {
    GMatchInfo* match_info;
    GRegex* regex;
    gchar* regex_str;
    gchar** matches;
    regex_str = "[^\/]+$";
    regex = g_regex_new(regex_str, 0, 0, &err);
    g_regex_match(regex, shell, 0, &match_info);
    shell = g_match_info_fetch(match_info, 0);
    g_match_info_unref(match_info);
    g_regex_unref(regex);
  }
  return shell;
}

gchar* read_file(gchar* path) {
  gchar* content;
  if (!g_file_get_contents(path, &content, NULL, NULL)) {
    content = "\n";
  }
  return content;
}

gboolean* write_file(gchar* path, gchar* content) {
  if (!g_file_set_contents(path, content, strlen(content), NULL)) {
    fprintf(stderr, "failed to write to %s", path);
    exit(1);
  }
  return TRUE;
}

gboolean* append_file(gchar* path, gchar* content) {
  FILE* f;
  f = fopen(path, "a");
  fprintf(f, "%s", content);
  fclose(f);
  return TRUE;
}

void print_hash_table(GHashTable* map) {
  GList* keys = g_hash_table_get_keys(map);
  GList* l;
  for (l = keys; l != NULL; l = l->next) {
    gchar* key;
    gchar* value;
    key = l->data;
    value = g_hash_table_lookup(map, key);
    g_printf("%s: %s\n", key, value);
  }
  g_list_free(l);
  g_list_free(keys);
}

void print_list(GList* list) {
  GList* l;
  for (l = list; l != NULL; l = l->next) {
    gchar* item;
    item = l->data;
    g_printf("-> %s\n", item);
  }
  g_list_free(l);
}

gchar* get_shell_path() {
  gchar* shell;
  gchar* shell_filename;
  gchar* shell_path;
  gchar* simlink_path;
  shell = get_shell();
  shell_filename = g_strconcat(".", shell, "rc", NULL);
  shell_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), shell_filename, NULL);
  simlink_path = g_file_read_link(shell_path, NULL);
  if (simlink_path) {
    shell_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), simlink_path, NULL);
  }
  return shell_path;
}

gchar* trim(gchar* value) {
  gint trim = 0;
  value = g_strstrip(value);
  if (value[0] == '"' && value[strlen(value)-1] == '"' && value[strlen(value)-2] != "\\\"") {
    trim = 1;
  } else if (value[0] == '\'' && value[strlen(value)-1] == '\'' && value[strlen(value)-2] != "\\\"") {
    trim = 1;
  }
  if (trim) {
    value = g_strndup(value+1, strlen(value)-2);
  }
  return value;
}

gboolean is_sourced(gchar* file_regex) {
  GError* err;
  GMatchInfo* match_info;
  GRegex* regex;
  gboolean sourced;
  gchar* content;
  gchar* regex_str;
  gchar* shell_path;
  err = NULL;
  sourced = FALSE;
  shell_path = get_shell_path();
  content = read_file(shell_path);
  regex_str = g_strconcat("([ \t]*#[ \t]*)?(source[ \t]+", file_regex, "[ \t\n]*)", NULL);
  regex = g_regex_new(regex_str, 0, 0, &err);
  g_regex_match(regex, content, 0, &match_info);
  while (g_match_info_matches(match_info)) {
    gchar** matches;
    matches = g_match_info_fetch_all(match_info);
    if (!matches[1] || strlen(matches[1]) <= 0) {
      sourced = TRUE;
    }
    g_match_info_next(match_info, NULL);
  }
  g_match_info_unref(match_info);
  g_regex_unref(regex);
  return sourced;
}

GList* uniq(GList* list) {
  GList *l, *item;
  GHashTable *table;
  table = g_hash_table_new(g_str_hash, g_str_equal);
  for (l = list; l;) {
    item = l;
    l = l->next;
    if (g_hash_table_contains(table, item->data)) {
      list = g_list_delete_link(list, item);
    } else {
      g_hash_table_add(table, item->data);
    }
  }
  g_list_free(l);
  g_hash_table_destroy(table);
  return list;
}

gint sort_chars_compf(gconstpointer a, gconstpointer b) {
  return strcmp((gchar*)a, (gchar*)b);
}

GList* sort_list(GList* list) {
  list = g_list_sort(list, sort_chars_compf);
  return list;
}

/* gchar* replace_env(gchar* env_key, gchar* string) { */
/*   gchar* env; */
/*   gchar* replaced; */
/*   GRegex* regex; */
/*   GRegexMatchFlags match_opts; */
/*   regex = g_regex_new(g_strconcat("\$", env_key, NULL), 0, 0, NULL); */
/*   env = g_getenv(env_key); */
/*   if (!env) env = ""; */
/*   replaced = g_regex_replace(regex, string, -1, 0, env, 0, NULL); */
/*   return replaced; */
/* } */

gchar* replace_envs(gchar* string) {
  /* GList* matches; */
  /* matches = regex("[$][A-Za-z0-9]+", string, PCRE_MULTILINE); */
  /* if (g_list_length(matches) >= 1) { */
  /*   g_printf("m %s\n", g_list_first(matches)->data); */
  /* } */


  /* gchar* replaced; */
  /* GRegex *regex; */
  /* GList* matches; */
  /* GMatchInfo** match_info; */
  /* replaced = string; */
  /* regex = g_regex_new("[$]", 0, 0, NULL); */
  /* g_regex_match_all_full(regex, string, -1, 0, 0, &match_info, NULL); */

  /* matches = NULL; */
  /* while (g_match_info_matches(match_info)) { */
  /*   g_match_info_next(match_info, NULL); */
  /*   g_printf("a"); */
  /* } */

  /* while (g_match_info_matches(match_info)) { */
  /*   gchar* result = g_match_info_fetch(match_info, 0); */
  /*   g_printf("m %s\n", result+1); */
  /*   /\* replaced = replace_env(result+1, replaced); *\/ */
  /*   g_match_info_next(match_info, NULL); */
  /*   g_free(result); */
  /* } */
  return "";
}

gchar* expand_path(gchar* path) {
  g_printf(":: %s\n", replace_envs("a $HOME is $good"));
  gchar* expanded_path = "hi";
  return expanded_path;
}
