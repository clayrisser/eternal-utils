#ifndef ETERNAL_UTILS_ALIASES_H
#define ETERNAL_UTILS_ALIASES_H

#include <glib.h>

GHashTable* get_aliases_from_content(char*);
GHashTable* get_eternal_aliases();
gchar* get_aliases_path();
gchar* get_content_from_aliases(GHashTable*);

#endif
