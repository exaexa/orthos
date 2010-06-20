
#include "confloader.h"
#include "skinloader.h"

#include "skin.h"

#include <stdio.h>
#include <string.h>

/*
 * dummy functions exported to the tested skin
 */

static int dummy_validate_login (const char*a, const char*b)
{
	printf ("skin checks auth for user `%s' password `%s'\n", a, b);
	return strcmp (a, b);
}

static int dummy_choose_login (const char*a, const char*b)
{
	printf ("skin sets login `%s' with session: %s\n", a, b);
	return 0;
}

static int dummy_choose_action (const char*a)
{
	printf ("skin wants action: %s\n", a);
	return 0;
}

static const char* dummy_get_config (const char*s)
{
	printf ("skin inspects setting `%s'\n", s);
	return config_get (s);
}

/*
 * main here exactly shows how the skins are handled (which can help)
 */

int main()
{
	void* skin;
	const char*fn;

	skin_init_func f_init;
	skin_fini_func f_fini;
	skin_run_func f_run;

	if (config_load() ) return 1;

	fn = config_get ("skin");
	if (!fn) return 2;

	printf ("using skin from %s\n", fn);

	skin = load_skin (fn, &f_init, &f_fini, &f_run);

	if (!skin) return 3;

	if (f_init (dummy_validate_login,
	            dummy_choose_login,
	            dummy_choose_action,
	            dummy_get_config) ) {

		printf ("skin reports failed initialization\n");
		goto termination;
	}

	if (f_run() ) {
		printf ("skin reports failed run\n");
		goto termination;
	}

	if (f_fini() ) printf ("skin reports failed shutdown\n");

termination:

	free_skin (skin);
	config_free();

	return 0;
}

