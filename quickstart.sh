#!/bin/sh
./autogen.sh &&
./configure --prefix=$PWD/install &&
make &&
make install &&
echo "config file will be created for you. Test the skin with command:" &&
echo "ORTHOS_CONF=./test.conf install/bin/oskintest" &&
cat > test.conf <<FEOF
Xserver	/usr/bin/Xorg vt07 -nolisten tcp
sessions	test:/usr/bin/test:clock:/usr/bin/xclock
skin	$PWD/install/lib/orthos/libshards.so
color 8:32:0
FEOF

