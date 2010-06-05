#!/bin/sh

# simple autogen script that generates basic layout for autotools.
# not meant to be included in distribution.

COMMON_CPPFLAGS="-I\$(srcdir)/include/ -I/usr/local/include"
COMMON_CFLAGS="-Wall"
COMMON_LDFLAGS="-L/usr/local/libs"

OUT=Makefile.am
touch NEWS AUTHORS ChangeLog
echo > $OUT
cd skins
PLUGINS=`echo *`
cd ..

echo "bin_PROGRAMS = orthos oskintest" >>$OUT
echo "pkglib_LTLIBRARIES = `for i in ${PLUGINS}; do echo -n \"lib$i.la \" ; done`" >>$OUT
echo "noinst_HEADERS = `echo include/*.h`" >>$OUT

echo "orthosdir = dm/" >>$OUT
echo "orthos_SOURCES = `echo dm/*.c`" >>$OUT
echo "orthos_CPPFLAGS = -I\$(srcdir)/dm/ ${COMMON_CPPFLAGS}" >>$OUT
echo "orthos_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
echo "orthos_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
echo "orthos_LDADD = -lX11 -lXmu -lcrypt -ldl " >>$OUT

echo "oskintestdir = src/" >>$OUT
echo "oskintest_SOURCES = `echo skintest/*.c`" >>$OUT
echo "oskintest_CPPFLAGS = -I\$(srcdir)/skintest/ ${COMMON_CPPFLAGS}" >>$OUT
echo "oskintest_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
echo "oskintest_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
echo "oskintest_LDADD = -ldl " >>$OUT

for i in $PLUGINS ; do
	echo "lib${i}_ladir = skins/${i}" >>$OUT
	echo "lib${i}_la_SOURCES = `echo skins/$i/*.c`" >>$OUT
	echo "noinst_HEADERS += `echo skins/$i/*.h |grep -v '*'`" >>$OUT
	echo "lib${i}_la_CPPFLAGS = -I\$(srcdir)/skins/$i/ ${COMMON_CPPFLAGS}" >>$OUT
	echo "lib${i}_la_CFLAGS = ${COMMON_CFLAGS}" >>$OUT
	echo "lib${i}_la_LDFLAGS = ${COMMON_LDFLAGS}" >>$OUT
	echo "lib${i}_la_LIBADD = " >>$OUT
	[ -f src/$i/Makefile.am.extra ] &&
		while read l ; do
			[ "$l" ] && echo "${i}_${l}" >>$OUT
		done < src/$i/Makefile.am.extra
done

libtoolize --force && aclocal && autoconf && automake --add-missing

