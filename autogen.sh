#!/bin/sh

# simple autogen script that generates basic layout for autotools.
# not meant to be included in distribution.

COMMON_CPPFLAGS="-I\$(srcdir)/include/ -I/usr/local/include"
COMMON_CFLAGS="-Wall"
COMMON_LDFLAGS="-L/usr/local/lib"

OUT=Makefile.am
touch NEWS AUTHORS ChangeLog
echo > $OUT
cd skins
PLUGINS=`echo *`
cd ..

echo "bin_PROGRAMS = orthos oskintest" >>$OUT
echo "pkglib_LTLIBRARIES = `for i in ${PLUGINS}; do echo -n \"lib$i.la \" ; done`" >>$OUT
echo "noinst_HEADERS = `echo \`find include/ -type f -name \*.h \` `" >>$OUT

echo "orthosdir = dm/" >>$OUT
echo "orthos_SOURCES = `echo \`find dm/ -type f -name \*.c \` `" >>$OUT
echo "noinst_HEADERS += `echo \`find dm/ -type f -name \*.h \` `" >>$OUT
echo "orthos_CPPFLAGS = -I\$(srcdir)/dm/ ${COMMON_CPPFLAGS}" >>$OUT
echo "orthos_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
echo "orthos_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
echo "orthos_LDADD = -lX11 -lXmu -lcrypt -ldl " >>$OUT

echo "oskintestdir = src/" >>$OUT
echo "oskintest_SOURCES = `echo \`find skintest/ -type f -name \*.c \` `" >>$OUT
echo "noinst_HEADERS += `echo \`find skintest/ -type f -name \*.h \` `" >>$OUT
echo "oskintest_CPPFLAGS = -I\$(srcdir)/skintest/ ${COMMON_CPPFLAGS}" >>$OUT
echo "oskintest_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
echo "oskintest_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
echo "oskintest_LDADD = -ldl " >>$OUT

for i in $PLUGINS ; do
	echo "lib${i}_ladir = skins/${i}" >>$OUT
	echo "lib${i}_la_SOURCES = `echo \`find skins/shards/ -type f -name \*.c -or -name \*.cpp\` `" >>$OUT
	echo "noinst_HEADERS += `echo \`find skins/shards/ -type f -name \*.h\` `" >>$OUT
	echo "lib${i}_la_CPPFLAGS = -I\$(srcdir)/skins/$i/ ${COMMON_CPPFLAGS}" >>$OUT
	echo "lib${i}_la_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
	echo "lib${i}_la_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
	echo "lib${i}_la_LIBADD = " >>$OUT
	[ -f skins/$i/Makefile.am.extra ] &&
		while read l ; do
			[ "$l" ] && echo "lib${i}_la_${l}" >>$OUT
		done < skins/$i/Makefile.am.extra
done

libtoolize --force && aclocal && autoconf && automake --add-missing

