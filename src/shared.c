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
