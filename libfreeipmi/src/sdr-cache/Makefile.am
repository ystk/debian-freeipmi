##*****************************************************************************
## $Id: Makefile.am,v 1.10 2008/09/17 22:56:05 chu11 Exp $
##*****************************************************************************
## Process this file with automake to produce Makefile.in.
##*****************************************************************************

noinst_HEADERS = \
	ipmi-sdr-cache-common.h \
	ipmi-sdr-cache-defs.h

noinst_LTLIBRARIES = libsdrcache.la 

libsdrcache_la_CPPFLAGS = \
	-I$(top_srcdir)/common/src/debugutil \
	-I$(top_srcdir)/common/src/miscutil \
	-I$(top_srcdir)/common/src/portability \
	-I$(top_srcdir)/libfreeipmi/include \
	-I$(top_srcdir)/libfreeipmi/src \
	-D_GNU_SOURCE \
	-D_REENTRANT

libsdrcache_la_SOURCES = \
	ipmi-sdr-cache.c \
	ipmi-sdr-cache-common.c \
	ipmi-sdr-cache-create.c \
	ipmi-sdr-cache-delete.c \
	ipmi-sdr-cache-read.c
