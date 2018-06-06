#ifndef ETERNAL_UTILS_UTILS_H
#define ETERNAL_UTILS_UTILS_H

#include <glib.h>

GList* regex(gchar*, gchar*, gint);
gchar* get_shell();
void print_hash_table(GHashTable*);
gchar* read_file(gchar*);

#endif
