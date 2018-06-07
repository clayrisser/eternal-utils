#include "aliases.h"

int main(int argc, char* argv[]) {
  GHashTable* aliases;
  aliases = get_eternal_aliases(argc, argv);
  print_hash_table(aliases);
  gchar* content;
  content = get_content_from_aliases(aliases);
  g_hash_table_destroy(aliases);
  g_printf("******************\n%s", content);
  g_free(content);
  return 0;
}
