
#include "xserver.h"

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define cookie_len 16
static char magic_cookie_hex[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

static int xserver_generate_xauth()
{
	int i, fd;
	char c;
	char p[] = "0123456789abcdef";

	/* we use system urandom for random bytes */

	fd = open ("/dev/urandom", O_RDONLY);
	for (i = 0;i < cookie_len;++i) {
		read (fd, &c, 1);
		magic_cookie_hex [2*i]   = p[  c     &0xf];
		magic_cookie_hex [2*i+1] = p[ (c>>4) &0xf];
	}
	close (fd);

	return 0;
}

int xserver_put_xauth (const char* file)
{

	return 0;
}

int xserver_export_user_env()
{

	return 0;
}

pid_t xserver_start()
{

	return 0;
}


int xserver_stop (pid_t pid)
{
	struct timespec tspec;

	killpg (pid, 15);

	/* we will now wait 5 seconds for X server to
	 * terminate, then kill it with 9. */

	tspec.tv_sec = 5;
	tspec.tv_nsec = 0;

	while ( waitpid (pid, 0, WNOHANG) < 0 )
		if (!nanosleep (&tspec, &tspec) ) {
			killpg (pid, 9);
			break;
		}
	pid = 0;
	return 0;
}

