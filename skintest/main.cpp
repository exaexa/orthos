
#include "skinload.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

bool skin_finished = false;

void get_x_size (int*x, int*y)
{
	XWindowAttributes attribs;
	*x = 800;
	*y = 600;
	Display*d = XOpenDisplay (0);
	if (!d) return;
	XGetWindowAttributes (d, DefaultRootWindow (d), &attribs);
	*x = attribs.width;
	*y = attribs.height;
	XCloseDisplay (d);
}

int dummy_validate_login (const char*a, const char*b)
{
	printf ("skin checks auth for user `%s' password `%s'\n", a, b);
	return strcmp (a, b);
}

int dummy_do_login (const char*a, const char*b)
{
	printf ("skin sets login `%s' with session: %s\n", a, b);
	return 0;
}

int dummy_action (const char*a)
{
	printf ("skin wants action: %s\n", a);
	skin_finished = true;
	return 0;
}

const char* dummy_get_config (const char*s)
{
	printf ("skin inspects setting `%s'\n", s);
	return get_setting (s);
}

int main (int argc, char**argv)
{
	if (argc < 2) {
		fprintf (stderr, "\
usage: %s <dummy-skin.so> [orthos-config]\n\
          `-' uses the config-specified file\n", argv[0]);
		return 1;
	}

	load_config (argc - 1, argv + 1);

	skin_init_func s_i;
	skin_fini_func s_f;
	skin_start_func s_start;
	skin_stop_func s_stop;
	skin_update_func s_update;

	if (load_skin ( ( (argv[1][0] == '-') && ! (argv[1][1]) ) ?
	                get_setting ("skin") : argv[1],
	                &s_i, &s_f, &s_start, &s_stop, &s_update) ) {
		fprintf (stderr, "skin loading failed\n");
		return 2;
	}

	int x, y;
	get_x_size (&x, &y);

	s_i (x, y, dummy_validate_login, dummy_do_login, dummy_action,
	     dummy_get_config);

	skin_finished = false;
	while (!skin_finished) {
		s_start();

		int r;
		while ( (r = s_update() ) == 1);
		if (r) printf ("some error occured, skin exited with %d\n", r);
		else printf ("skin gracefully exits the loop\n");

		s_stop();
	}
	s_f();

	free_skin();
	free_config();
	return 0;
}

