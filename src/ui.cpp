
#include "ui.h"
#include "skinload.h"
#include "settings.h"
#include "sys.h"
#include "orthos.h"

#include <iostream>
using namespace std;

static skin_init_func s_init;
static skin_fini_func s_fini;
static skin_start_func s_start;
static skin_stop_func s_stop;
static skin_update_func s_update;

int ui_init()
{
	if(load_skin(get_setting("skin"),
		&s_init, &s_fini, &s_start, &s_stop, &s_update))
		return 1;
	
	int x=0,y=0;
	x_get_resolution(&x,&y);

	s_init(x,y,orthos_validate_login,
		orthos_do_login, orthos_action,
		orthos_get_session_list);
	
	return 0;
}

int ui_release()
{
	s_fini();
	free_skin();
	return 0;
}

/*
 * in this, we desperately NEED to handle XIO errors,
 * so they dont shot orthos off. Using setjmp() technique.
 */

#include <setjmp.h>

static jmp_buf jb;

#include <X11/Xlib.h>

static bool reinit_next=false;

#include <stdio.h>

int handler(Display*d)
{
	printf("handling it\n");
	longjmp(jb, 1);
}

int ui_run()
{
	if(setjmp(jb)){
		printf("fini skin\n");
		reinit_next=true;
		ui_release();
		return 0;
	}

	if(reinit_next){
		printf("reload skin\n");
		ui_init();
		reinit_next=false;
	}
	
	printf("starting skin\n");

	if(s_start()) {
		printf("start died\n");
		return 1;
	}

	printf("running skin\n");
	int r=0;
	while((r=s_update())==1);
	if(r)return 2; //update exited with error status
	printf("stopping skin\n");
	s_stop();
	XSetIOErrorHandler(0);
	return 0;
}

