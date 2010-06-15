
#include "skinloader.h"
#include "confloader.h"

#include "skin.h"

#include "stdio.h"

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

	skin = load_skin (fn, &f_init, &f_fini, &f_run);

	if (!skin) return 3;

	/* TODO */

	free_skin (skin);
	config_free();

	return 0;
}
