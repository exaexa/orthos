#!/bin/sh
CP='cp -v'
pfx=${PREFIX:-/usr/local}
dest=${DEST:-}
scons
$CP ./orthos ${dest}${pfx}/bin/orthos
$CP ./oskintest ${dest}${pfx}/bin/oskintest
mkdir -p ${dest}${pfx}/lib/orthos
$CP ./skins/*.so ${dest}${pfx}/lib/orthos/

conffile="${dest}/etc/orthos.conf"
if [ -f "$conffile" ] ; then
	echo "config file found, not creating it"
else
	echo "creating default config file"
	echo > ${conffile}
	echo -e "sessions\tfailsafe:/usr/bin/xterm:clock:/usr/bin/xclock" >> ${conffile}
	echo -e "Xserver\t/usr/bin/Xorg" >> ${conffile}
	echo -e "skin\t$pfx/lib/orthos/libdefault.so" >> ${conffile}
fi

