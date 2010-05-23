
#include "exa-base.h"
#include "OGLFT.h"

#include "skin.h"

#include <math.h>

#include <sys/utsname.h> //for uname/hostname.

#include <string>
#include <vector>
#include <iostream>
using namespace std;

static orthos_validate_login_func validate_login;
static orthos_do_login_func do_login;
static orthos_action_func action;
static orthos_get_config_func config;

static int x = 0, y = 0;

static float Cred = 0.1, Cgreen = 0.1, Cblue = 0.1;
bool invert;

#include "vector.h"


string login_line;

class Session
{
public:
	string name, cmd;
	Session (const string&n, const string&c) {
		name = n;
		cmd = c;
	}
	Session (const char*n, const char*c) {
		name = n;
		cmd = c;
	}
};

vector<Session> sessions;
int selected_session=1;

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


int orthos_skin_update()
{
	exaUpdate();
	float time = 0;

	while (time < 0.035) {
		time += exaGetElapsedTime();
		exaUSleep (2000);
	}

	if(exaIsKeyDown(EKEY_ESCAPE))return 1;
	else return 0;
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

	int r = 6, g = 16, b = 32;

	if (config ("color") ) sscanf (config ("color"),
		                               "%d:%d:%d", &r, &g, &b);
	Cred = (float) r / 255.0;
	Cgreen = (float) g / 255.0;
	Cblue = (float) b / 255.0;

	invert = false;
	if (config ("invert") ) if (config ("invert") [0] == 'y') invert = true;

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

int orthos_skin_start()
{
	exaSetParams (x, y, 24);
	if (!exaInit() ) return 1;

	exaKeyRepeat (0.4, 0.05);

	const char*font_fn = config ("font");
	const char*font_alt_fn= config ("font-alt");

	glClearColor (0, 0, 0, 0);
	glShadeModel (GL_SMOOTH);
	glDisable (GL_DEPTH_TEST);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glFrustum (-0.16, 0.16, -0.1, 0.1, 0.1, 100);
	glMatrixMode (GL_MODELVIEW);

	exaGetElapsedTime();
	return 0;
}


int orthos_skin_stop()
{
	exaShutdown();
	return 0;
}

