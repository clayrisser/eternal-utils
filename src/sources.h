#ifndef ETERNAL_SOURCES_ALIASES_H
#define ETERNAL_SOURCES_ALIASES_H

#include <glib.h>

GList* get_sources_from_args(gint, gchar* argv[], GList*);
GList* get_eternal_sources();
gboolean* write_sources(GList*);

#endif
