
#include "dm.h"
#include "util.h"
#include "version.h"
#include "confloader.h"

#include <stdio.h>

#define INFO_STRING \
"\n" \
"  orthos the versatile X11 display manager version " ORTHOS_VERSION "\n" \
"  is distributed and shall be used under the terms of GNU GPLv3.\n" \
"\n" \
"  To correctly startup orthos, please provide default configuration file at\n"\
"   " DEFAULT_ORTHOS_CONF "\n" \
"  or select yours by setting environment variable like this:\n" \
"   export ORTHOS_CONF=/my/o.conf\n" \
"\n" \
"  More documentation should be available at manual page orthos(1).\n" \
"\n"

int main(int argc, char**argv)
{
	if(argc>1) {
		printf("error: expected to run without parameters.\n" INFO_STRING);
		return 1;
	}

	if(config_load()) {
		printf("error: loading configuration file failed.\n" INFO_STRING);
		return 2;
	}

	if(util_daemonize()) {
		printf("error: couldn't daemonize");
		return 3;
	}

	return dm_run();
}
