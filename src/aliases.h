#ifndef ETERNAL_UTILS_ALIASES_H
#define ETERNAL_UTILS_ALIASES_H

#include <glib.h>

GHashTable* get_aliases_from_args(gint, gchar* argv[], GHashTable*);
GHashTable* get_eternal_aliases();
GHashTable* unalias_eternal_aliases(gint, gchar* argv[], GHashTable*);
gboolean* write_aliases(GHashTable*);

#endif
