bin_PROGRAMS = locopdf
locopdf_SOURCES = locopdf.cpp

# set the include path found by configure
INCLUDES= $(all_includes) @ECORE_CFLAGS@ @EPDF_CFLAGS@ @EVAS_CFLAGS@

# the library search path.
locopdf_LDFLAGS = $(all_libraries) @ECORE_LIBS@ @EVAS_LIBS@ @EPDF_LIBS@ -lecore_evas
