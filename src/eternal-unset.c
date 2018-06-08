#include "envs.h"

gint main(gint argc, gchar** argv) {
  GHashTable* envs;
  envs = get_eternal_envs();
  envs = unset_eternal_envs(argc, argv, envs);
  write_envs(envs);
  g_hash_table_destroy(envs);
  return 0;
}
