#ifndef ETERNAL_UTILS_UTILS_H
#define ETERNAL_UTILS_UTILS_H

#include <glib.h>

GList* regex(gchar*, gchar*, gint);
gboolean* write_file(gchar*, gchar*);
gchar* get_shell();
gchar* read_file(gchar*);
gchar* trim(gchar*);
void print_hash_table(GHashTable*);

#endif
