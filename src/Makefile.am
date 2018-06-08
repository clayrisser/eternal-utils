AM_CFLAGS = --pedantic -Wall -std=c99 -O2 $(GLOB_CFLAGS)
AM_LDFLAGS =
LDADD = $(GLOB_LIBS)
bin_PROGRAMS = $(top_builddir)/bin/eternal-alias \
	$(top_builddir)/bin/eternal-export \
	$(top_builddir)/bin/eternal-source \
	$(top_builddir)/bin/eternal-unalias \
	$(top_builddir)/bin/eternal-unset \
	$(top_builddir)/bin/eternal-unsource
__top_builddir__bin_eternal_alias_SOURCES = shared.h shared.c aliases.h aliases.c eternal-alias.c
__top_builddir__bin_eternal_export_SOURCES = shared.h shared.c envs.h envs.c eternal-export.c
__top_builddir__bin_eternal_source_SOURCES = shared.h shared.c sources.h sources.c eternal-source.c
__top_builddir__bin_eternal_unalias_SOURCES = shared.h shared.c aliases.h aliases.c eternal-unalias.c
__top_builddir__bin_eternal_unset_SOURCES = shared.h shared.c envs.h envs.c eternal-unset.c
__top_builddir__bin_eternal_unsource_SOURCES = shared.h shared.c sources.h sources.c eternal-unsource.c
