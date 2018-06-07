#ifndef ETERNAL_UTILS_ENVS_H
#define ETERNAL_UTILS_ENVS_H

#include <glib.h>

GHashTable* get_eternal_envs(gint, gchar* argv[]);
gchar* get_content_from_envs(GHashTable*);

#endif
