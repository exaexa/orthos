
#include "dm.h"

#include "xserver.h"
#include "confloader.h"
#include "skinloader.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int dm_session_validate_login (const char*login, const char*pw)
{

	return 0;
}

int dm_session_set_login (const char*login, const char*session)
{

	return 0;
}

int dm_session_set_action (const char*command)
{

	return 0;
}

const char* dm_session_get_config (const char*name)
{
	return config_get (name);
}

/*
 * dm_session
 * to us, a session means one skin run with possible user session run.
 * Note that DM exits if any action is selected.
 */

int dm_session()
{
	/* TODO */
	return 0;
}

/*
 * handling of DM signals
 */

int g_killed = 0;
int g_restart = 0;

void signal_handler (int sig)
{
	if (sig == SIGTERM) g_killed = 1;
	if (sig == SIGQUIT) g_killed = 1;
	if (sig == SIGUSR1) g_restart = 1;
}

/*
 * dm_run
 * the function that does it all. Roughly this:
 *
 * - loads skin
 * - cycles starting X server and spawns a session
 * - when detects problems or exit signal, gives up (optionally
 *   terminating the server)
 * - unloads skin
 */

int dm_run()
{
	/*
	 * dm does this:
	 * - loads skin
	 * - cycles starting X server and spawns a session
	 * - when detects problems or exit signal, gives up (optionally
	 *   terminating the server)
	 * - unloads skin
	 */

	void*skin;

	pid_t xserver_pid;
	pid_t session_pid;

	skin_init_func sk_init;
	skin_run_func sk_run;
	skin_fini_func sk_fini;

	const char* t;

	/*
	 * signal setup
	 *
	 * note that USR1 is used to kill the current session
	 */

	signal (SIGTERM, signal_handler);
	signal (SIGQUIT, signal_handler);
	signal (SIGUSR1, signal_handler);

	/*
	 * skin initialization
	 */

	t = config_get ("skin");
	if (!t) {
		printf ("no skin selected\n");
		return 1;
	}

	skin = load_skin (t, &sk_init, &sk_fini, &sk_run);

	if (!skin) {
		printf ("skin loading failure\n");
		return 2;
	}

	if (sk_init (dm_session_validate_login, dm_session_set_login,
	             dm_session_set_action, dm_session_get_config) ) {

		printf ("skin initialization failed\n");
		return 3;
	}

	while (!g_killed) {

		g_restart = 0;

		xserver_pid = xserver_start();

		if (!xserver_pid) {
			printf ("failed to start X server\n");
			break;
		}

		session_pid = fork();
		if (session_pid < 0) {
			printf ("failed to fork off a session\n");
			break;
		} else if (session_pid == 0) {

			/*
			 * child process. Run the session and die.
			 */

			_exit (dm_session() );
		}

		/*
		 * Wait for session or X server to die. Kill the rest, then
		 * restart the cycle. The thing that can happen is that we get
		 * killed by some other interesting signal, in that case we
		 * terminate gracefully.
		 */

		while (! (g_killed || g_restart) ) {

			if (waitpid (session_pid, 0, WNOHANG) >= 0 ) {
				session_pid = 0;
				g_restart = 1;

			} else if (waitpid (xserver_pid, 0, WNOHANG) >= 0 ) {
				xserver_pid = 0;
				g_restart = 1;

			} else
				wait (0); /* wait for more awesome signals */
		}

		/* kill xserver and session. Be brutal to session, on purpose */

		if (session_pid) {
			kill (session_pid, 9);
			session_pid = 0;
		}

		if (xserver_pid) {
			xserver_stop (xserver_pid);
			xserver_pid = 0;
		}
	}

	/* termination phase */

	sk_fini();
	free_skin (skin);

	return 0;
}
