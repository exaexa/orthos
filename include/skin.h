#ifndef ORTHOS_SKIN_H
#define ORTHOS_SKIN_H

#ifdef __cplusplus
extern "C"
{
#endif

	typedef int (*orthos_validate_login_func) (const char*, const char*);
	typedef int (*orthos_do_login_func) (const char*, const char*);
	typedef int (*orthos_action_func) (const char*);
	typedef const char* (*orthos_get_config_func) (const char*);

	typedef int (*skin_init_func) (orthos_validate_login_func,
	                               orthos_do_login_func,
	                               orthos_action_func,
	                               orthos_get_config_func);
	typedef int (*skin_fini_func) ();
	typedef int (*skin_run_func) ();


	int orthos_skin_init (orthos_validate_login_func,
	                      orthos_do_login_func,
	                      orthos_action_func,
	                      orthos_get_config_func);
	int orthos_skin_fini();

	int orthos_skin_run();


	int orthos_validate_login (const char*name, const char*pw);
	int orthos_choose_login (const char*name, const char* session);
	int orthos_choose_action (const char* command);
	const char* orthos_get_config (const char*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif


