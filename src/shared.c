#include <glib.h>
#include <pcre.h>
#include <stdio.h>

gchar* find_all(gchar* regex, gchar* string) {
  const int MATCHES_COUNT = 3;
  gchar* error;
  gchar* match;
  int matches[MATCHES_COUNT];
  int errorOffset;
  int rc;
  pcre* re;
  re = pcre_compile(regex, PCRE_MULTILINE, &error, &errorOffset, 0);
  if (!re) {
    fprintf(stderr, "failed to compile regex: %s\n", error);
    exit(1);
  }
  rc = pcre_exec(re, NULL, string, strlen(string), 0, 0, matches, MATCHES_COUNT);
  if (rc < 0) {
    switch(rc) {
    case PCRE_ERROR_NOMATCH:
      match = "";
      break;
    default:
      fprintf(stderr, "regex match failed");
      exit(1);
      break;
    }
  } else {
    match = string+matches[0];
  }
  gchar* result = malloc(strlen(match) + 1);
  strcpy(result, match);
  pcre_free(re);
  return result;
}

gchar* get_shell() {
  gchar* shell;
  shell = getenv("SHELL");
  if (!shell) {
    shell = "bash";
  } else {
    gchar* match = find_all("[^\/]+$", shell);
    if (strlen(shell) <= 0) {
      shell = "bash";
    } else {
      strcpy(shell, match);
    }
    g_free(match);
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
