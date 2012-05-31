ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

include aminclude.am


lib_LTLIBRARIES = libprosopon.la

# libprosopon.la
libprosopon_la_CFLAGS = -std=c99 -g -I./include

libprosopon_la_SOURCES = src/pro_actor.c src/pro_list.c src/pro_lookup.c\
    src/pro_object.c src/pro_state.c src/pro_library.c src/pro_message_queue.c\
    src/pro_type.c src/pro_user_data.c src/pro_messaging.c src/pro_lookup_list.c

library_includedir = $(includedir)/prosopon
library_include_HEADERS = include/prosopon/prosopon.h include/prosopon/actor_type.h \
    include/prosopon/config.h include/prosopon/library.h include/prosopon/core.h \
    include/prosopon/macros.h


# Extra Dist
EXTRA_DIST = autogen.sh
