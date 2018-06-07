#include "envs.h"

int main(int argc, char* argv[]) {
  GHashTable* envs;
  envs = get_eternal_envs(argc, argv);
  print_hash_table(envs);
  gchar* content;
  content = get_content_from_envs(envs);
  g_hash_table_destroy(envs);
  g_printf("******************\n%s", content);
  g_free(content);
  return 0;
}
