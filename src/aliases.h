#ifndef ETERNAL_UTILS_ALIASES_H
#define ETERNAL_UTILS_ALIASES_H

#include <glib.h>

GHashTable* get_eternal_aliases(gint, gchar* argv[]);
gboolean* write_aliases(GHashTable*);

#endif
