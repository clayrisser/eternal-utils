#include <glib.h>
#include <pcre.h>
#include <stdio.h>

GList* regex(gchar* regex, gchar* string, gint options) {
  const gchar* errMessage;
  const gint OVECCOUNT = 30;
  GList* matches = NULL;
  gint errOffset;
  gint ovector[OVECCOUNT];
  gint matches_count;
  pcre* re;
  re = pcre_compile(regex, options, &errMessage, &errOffset, 0);
  if (!re) {
    fprintf(stderr, "%s", errMessage);
    exit(1);
  }
  matches_count = pcre_exec(re, NULL, string, strlen(string), 0, 0, ovector, OVECCOUNT);
  if (matches_count < 0) {
    switch(matches_count) {
    case PCRE_ERROR_NOMATCH:
      break;
    default:
      fprintf(stderr, "regex match failed");
      exit(1);
      break;
    }
  } else {
    for (int i = 0; i < matches_count; i++) {
      char *start;
      gchar* match;
      int length;
      start = string + ovector[2*i];
      length = ovector[2*i+1] - ovector[2*i];
      match = g_strndup(start, length);
      matches = g_list_append(matches, match);
    }
  }
  return matches;
}

gchar* get_shell() {
  gchar* shell;
  shell = getenv("SHELL");
  if (!shell) {
    shell = "bash";
  } else {
    GList* matches;
    matches = regex("[^\/]+$", shell, PCRE_MULTILINE);
    if (matches == NULL) {
      shell = "bash";
    } else {
      strcpy(shell, g_list_first(matches)->data);
    }
    g_list_free(matches);
  }
  gchar* result = malloc(strlen(shell) + 1);
  strcpy(result, shell);
  return result;
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

gchar* get_aliases_path() {
  gchar* aliases_filename;
  gchar* aliases_path;
  gchar* shell;
  shell = get_shell();
  aliases_filename = g_strconcat(".", shell, "_aliases", NULL);
  aliases_path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), aliases_filename, NULL);
  g_free(shell);
  return aliases_path;
}

gchar* read_file(gchar* path) {
  gchar* content;
  GError* err;
  g_file_get_contents(path, &content, NULL, &err);
  if (err != NULL) {
    if (err->code == G_FILE_ERROR_NOENT) {
      content = "\n";
    } else {
      g_printf("code: %i\n", err->code);
      fprintf(stderr, err->message);
    }
    g_error_free(err);
  }
  gchar* result = malloc(strlen(content) + 1);
  strcpy(result, content);
  return result;
}

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

void get_envs() {
  gchar* content;
  gchar* envs_path;
  GHashTable* envs;
  envs_path = get_envs_path();
  content = read_file(envs_path);
  envs = get_envs_from_content(content);
  print_hash_table(envs);
  g_free(envs_path);
  g_free(content);
  return envs;
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
}
