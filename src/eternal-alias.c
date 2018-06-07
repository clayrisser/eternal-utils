#include "aliases.h"

int main(int argc, char* argv[]) {
  GHashTable* aliases;
  aliases = get_eternal_aliases();
  aliases = get_aliases_from_args(argc, argv, aliases);
  write_aliases(aliases);
  g_hash_table_destroy(aliases);
  return 0;
}
