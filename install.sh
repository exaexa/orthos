#!/bin/sh
CP='cp -v'
pfx=${PREFIX:-/usr/local}
scons
$CP ./orthos $pfx/bin/orthos
$CP ./oskintest $pfx/bin/oskintest
mkdir -p $pfx/lib/orthos
$CP ./skins/*.so $pfx/lib/orthos/
echo > /etc/orthos.conf
echo -e "sessions\tfailsafe:/usr/bin/xterm:clock:/usr/bin/xclock" >>/etc/orthos.conf
echo -e "Xserver\t/usr/bin/Xorg" >>/etc/orthos.conf
echo -e "skin\t$pfx/lib/orthos/libdefault.so" >>/etc/orthos.conf

