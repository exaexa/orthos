
#ifndef ORTHOS_H
#define ORTHOS_H

int orthos_main (int argc, char**argv);

int orthos_check_auth (char*username, char*pw);

//these 2 funcs are called from inside UI to tell the core
//what should happen after the shiny interface disappears
int orthos_set_login (char*username, char*session);
int orthos_set_action (char*command);
int orthos_kill (int);

#endif

