
#include "orthos.h"
#include "skinload.h"
#include "settings.h"
#include "sys.h"
#include "ui.h"

#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include <string>
#include <iostream>
using namespace std;

static int g_action = 0;
#define action_login 1
#define action_command 2
static string g_login_name, g_command;

static int g_killed = 0;

int orthos_main (int argc, char**argv)
{
	load_config (argc, argv);

	{
		int t = fork_to_background();
		if (t > 0) return 0;
		if (t < 0) return 1;
	}

	sys_setup();

	if (x_server_start() ) goto terminate;

	//if(x_server_running()) goto terminate;

	if (ui_init() ) goto terminate;

	while (!g_killed) {
		int result, r;
		int worker = fork();
		printf ("forked off %d\n", worker);
		if (worker < 0) goto terminate;
		if (!worker) {
			/*exit codes are:
			 * 1 - fukken die
			 * anything else - retry
			 */
			ui_run();
			printf ("UI ok\n");
			if (x_server_running() ) _exit (1);
			switch (g_action) {
			case action_login:
				sys_do_login_user (
				    g_login_name.c_str(),
				    g_command.c_str() );
				_exit (2);
			case action_command:
				sys_spawn (g_command.c_str() );
				_exit (1);
			default:
				_exit (3);
			}
		}

		while (1) {
			r = waitpid (worker, &result, 0);
			printf ("waitpid\n");
			if (r <= 0) if (errno == EINTR) {
					if (g_killed) {
						kill (worker, SIGTERM);
						printf ("killed, killing\n");
					} else continue;
				}
			break;
		}
		printf ("waitpid done");
		if ( (r <= 0) || g_killed) goto terminate;
		printf ("result\n");
		switch (result) {
		case 1:
			goto terminate;
		default:
			if (x_server_running() ) {
				printf ("not running\n");
				if (x_server_start() ) goto terminate;
			} else if (get_bool_setting ("restart_after_session") ) {
				x_server_stop();
				if (x_server_start() ) goto terminate;
			}
		}
	}

terminate:
	printf ("termination\n");
	ui_release();
	x_server_stop();
	sys_reset_signals();
	free_config();
	printf ("ok\n");
	return 0;
}


int orthos_kill (int signal)
{
	return g_killed = signal;
}


/*
 * following functions are exported to the plugin.
 */

#include "skin.h"

int orthos_validate_login (const char*name, const char*pw)
{
	return sys_auth_user (name, pw);
}

int orthos_do_login (const char*name, const char*session)
{
	g_action = action_login;
	g_login_name = name;
	g_command = session;
	return 0;
}

int orthos_action (const char*command)
{
	g_action = action_command;
	g_command = command;
	return 0;
}

const char* orthos_get_config (const char*x)
{
	return get_setting (x);
}

