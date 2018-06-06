#include "envs.h"

int main(int argc, char* argv[]) {
  GHashTable* envs;
  envs = get_eternal_envs();
  print_hash_table(envs);
  return 0;
}
