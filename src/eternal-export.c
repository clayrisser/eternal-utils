#include "envs.h"

gint main(gint argc, gchar** argv) {
  GHashTable* envs;
  envs = get_eternal_envs();
  envs = get_envs_from_args(argc, argv, envs);
  write_envs(envs);
  g_hash_table_destroy(envs);
  return 0;
}
