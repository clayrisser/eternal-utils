#include "sources.h"

gint main(gint argc, gchar* argv[]) {
  GList* sources;
  sources = get_eternal_sources();
  sources = unsource_eternal_sources(argc, argv, sources);
  write_sources(sources);
  g_list_free(sources);
  return 0;
}
