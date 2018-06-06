#ifndef ETERNAL_UTILS_UTILS_H
#define ETERNAL_UTILS_UTILS_H

#include <glib.h>

GList* regex(gchar* regex, gchar* string, gint options);
gchar* get_shell();
void print_hash_table(GHashTable* map);
gchar* read_file(gchar* path);

#endif
