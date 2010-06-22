
#include "util.h"
#include "confloader.h"

#define _XOPEN_SOURCE
#include <unistd.h>

int util_daemonize()
{
	int debug=config_get_bool("debug");

	if(debug==1)
		return daemon(0,1);
	else	return daemon(0,0);
}

