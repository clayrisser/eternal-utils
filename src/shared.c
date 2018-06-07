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
  pcre_free(re);
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
  return shell;
}

gchar* read_file(gchar* path) {
  gchar* content;
  if (!g_file_get_contents(path, &content, NULL, NULL)) {
    content = "\n";
  }
  return content;
}

gboolean write_file(gchar* path, gchar* content) {
  if (!g_file_set_contents(path, content, strlen(content), NULL)) {
    fprintf(stderr, "failed to write to %s", path);
    exit(1);
  }
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
  int trim = 0;
  if (value[0] == '"' && value[strlen(value)-1] == '"' && value[strlen(value)-2] != "\\\"") {
    trim = 1;
  } else if (value[0] == '\'' && value[strlen(value)-1] == '\'' && value[strlen(value)-2] != "\\\"") {
    trim = 1;
  }
  if (trim) {
    g_utf8_strncpy(value, value+1, strlen(value)-2);
  }
  return value;
}

gboolean is_sourced(gchar* file_regex) {
  GList* matches;
  gchar* content;
  gchar* regex_str;
  gchar* shell_path;
  gboolean sourced;
  sourced = FALSE;
  shell_path = get_shell_path();
  content = read_file(shell_path);
  regex_str = g_strconcat("([ \t]*#[ \t]*)?(source[ \t]+", file_regex, ")", NULL);
  matches = regex(regex_str, content, 0);
  if (g_list_length(matches) >= 3) {
    if (strlen(g_list_nth(matches, 1)->data) <= 0) {
      if (strlen(g_list_nth(matches, 2)->data) > 0) {
        sourced = TRUE;
      }
    }
  }
  return sourced;
}
