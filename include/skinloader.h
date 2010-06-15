
#ifndef ORTHOS_SKINLOADER_H
#define ORTHOS_SKINLOADER_H

/*
 * this is a simple routine in header file. Compiler nazis suffer.
 */

#include "skin.h"

#ifdef SKINLOADER_IMPL

#include <dlfcn.h>
#include <stdio.h>

void* orthos_load_skin (const char* filename,
                        skin_init_func*a, skin_fini_func*b, skin_run_func*c)
{
	void*h;
	h = dlopen (filename, RTLD_NOW);
	if (!h) return 0;

	*a = dlsym (h, "orthos_skin_init");
	*b = dlsym (h, "orthos_skin_fini");
	*c = dlsym (h, "orthos_skin_run");

	return h;
}

void orthos_free_skin (void* handle)
{
	dlclose (h);
}

#else

void* orthos_load_skin (const char*,
                        skin_init_func*, skin_fini_func*, skin_run_func*)
void orthos_free_skin (void*)

#endif

#endif

