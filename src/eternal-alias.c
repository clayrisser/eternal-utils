#include "aliases.h"

int main(int argc, char* argv[]) {
  GHashTable* aliases;
  aliases = get_eternal_aliases(argc, argv);
  gchar* content;
  content = get_content_from_aliases(aliases);
  g_hash_table_destroy(aliases);
  return 0;
}
