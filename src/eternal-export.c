#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include "shared.h"

int main(int argc, char* argv[]) {
  /* gchar* envs_path = get_envs_path(); */
  /* g_printf("%s", envs_path); */
  /* g_free(envs_path); */
  get_envs();
  return 0;
}
