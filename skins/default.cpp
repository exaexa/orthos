
#include "exa-base.h"
#include "exa-font.h"

#include "skin.h"

#include <math.h>

#include <sys/utsname.h> //for uname/hostname.

#include <string>
#include <vector>
#include <iostream>
using namespace std;

exaGLFont font;
string login_line;

int Mode; //login/pw/session/wait
int Cred = 0, Cgreen = 25, Cblue = 76;

static orthos_validate_login_func validate_login;
static orthos_do_login_func do_login;
static orthos_action_func action;
static orthos_get_config_func config;

static int x = 0, y = 0;

#include "simple-loginbox.h"

InputBox box;
SessionBox sess;
ShutdownBox shut;

int parse_sessions (const char*s)
{
	sessions.clear();
	sessions.push_back (Session ("custom", "") );
	selected_session = 1;
	string a, n;
	bool first = true;
	while (*s) {
		if (*s == ':') {
			if (first) {
				n = a;
				a.clear();
			} else {
				sessions.push_back (Session (n, a) );
				n.clear();
				a.clear();
			}
			first = !first;
		} else a.append (1, *s);
		++s;
	}
	if (!first) sessions.push_back (Session (n, a) );

	return 0;
}

int orthos_skin_start()
{
	//----

	exaSetParams (x, y, 24);
	if (!exaInit() ) return 1;

	exaKeyRepeat (0.4, 0.05);

	const char*font_fn = config ("font");

	font.loadfromfreetype (font_fn ? font_fn :
	                       "/usr/share/fonts/ttf-bitstream-vera/VeraMono.ttf", 20);

	box.setcaption (login_line);

	glClearColor (0, 0, 0, 0);
	glShadeModel (GL_SMOOTH);
	glDisable (GL_DEPTH_TEST);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glFrustum (-0.16, 0.16, -0.1, 0.1, 0.1, 100);
	glMatrixMode (GL_MODELVIEW);


	Mode = 0;
	box.setcaption (login_line);
	box.pwmask (false);
	box.setcolor (0.4, 0.4, 0.4);
	box.flash (0, 0, 0);
	exaGetElapsedTime();
	return 0;
}


float rot;
string login, pw, session;

int orthos_skin_update()
{
	exaUpdate();
	float time = 0;

	while (time < 0.02) {
		time += exaGetElapsedTime();
		exaUSleep (2000);
	}

	rot += time;
	box.update (time);
	sess.update (time);
	shut.update (time);

	if (exaIsKeyHit (EKEY_RETURN) )
		switch (Mode) {
		case 0:
			login = box.text;
			Mode = 1;
			box.clear();
			box.setcaption ("password");
			box.setcolor (0.2, 0.2, 0.2);
			box.pwmask (true);
			break;
		case 1:
			pw = box.text;
			box.clear();
			if (selected_session) {
				if (validate_login (login.c_str(), pw.c_str() ) ) {
					Mode = 0;
					box.setcaption (login_line);
					box.setcolor (0.4, 0.4, 0.4);
					box.flash (1, 0, 0);
					box.pwmask (false);
				} else Mode = 3;
			} else {
				Mode = 2;
				box.setcaption ("session");
				box.setcolor (0.1, 0.1, 0.4);
				box.pwmask (false);
			}
			break;
		case 2:
			session = box.text;
			box.clear();
			if (validate_login (login.c_str(), pw.c_str() ) ) {
				Mode = 0;
				box.setcaption (login_line);
				box.setcolor (0.4, 0.4, 0.4);
				box.flash (1, 0, 0);
				box.pwmask (false);
			} else Mode = 3;
			break;
		case 3:
		case 4:
			break;
		default:
			Mode = 0;
			box.clear();
			box.setcaption (login_line);
			box.pwmask (false);
			box.setcolor (0.4, 0.4, 0.4);
			box.flash (0, 0, 0);
			break;
		}

	glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef (0, 0, -5);
	glRotatef (rot, 0, 0, 1);
	for (int i = 0;i < 10;++i) {
		glBegin (GL_TRIANGLE_STRIP);
		glColor3f (0, 0, 0);
		glVertex3f (1, 5, 5);
		glVertex3f (-1, 5, 5);
		glColor3ub (Cred, Cgreen, Cblue);
		glVertex3f (1, 5, -1);
		glVertex3f (-1, 5, -1);
		glColor3f (0, 0, 0);
		glVertex3f (1, 5, -20);
		glVertex3f (-1, 5, -20);
		glEnd();
		glRotatef (36, 0, 0, 1);
	}

	glLoadIdentity();
	glTranslatef (0, 0, -25);
	box.draw();
	glTranslatef (0, 2, 0);
	sess.draw();
	glTranslatef (0, -6, 0);
	shut.draw();

	exaglSwapBuffers();
	switch (Mode) {
	case 3:
		do_login (login.c_str(), selected_session ? sessions[selected_session].cmd.c_str() : session.c_str() );
		return 0;
	case 4:
		return 0;
	}
	return 1;
}

int orthos_skin_init (int X, int Y,
                      orthos_validate_login_func f1,
                      orthos_do_login_func f2,
                      orthos_action_func f3,
                      orthos_get_config_func f4)
{
	x = X;
	y = Y;
	validate_login = f1;
	do_login = f2;
	action = f3;
	config = f4;

	parse_sessions (config ("sessions") );

	if (config ("color") ) sscanf (config ("color"),
		                               "%d:%d:%d", &Cred, &Cgreen, &Cblue);

	//login_line=string(getenv("HOSTNAME"))+" login";
	struct utsname u;
	uname (&u);
	login_line = string (u.nodename) + " login";
	return 0;
}

int orthos_skin_fini()
{

	return 0;
}

int orthos_skin_stop()
{
	font.unload();
	exaShutdown();
	return 0;
}

