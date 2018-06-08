#ifndef ETERNAL_UTILS_ENVS_H
#define ETERNAL_UTILS_ENVS_H

#include <glib.h>

GHashTable* get_envs_from_args(gint, gchar**, GHashTable*);
GHashTable* get_eternal_envs();
GHashTable* unset_eternal_envs(gint, gchar**, GHashTable*);
gboolean* write_envs(GHashTable*);

#endif
