
#ifndef ORTHOS_CONFREADER_H
#define ORTHOS_CONFREADER_H

#ifndef DEFAULT_ORTHOS_CONF
#define DEFAULT_ORTHOS_CONF "/etc/orthos.conf"
#endif

#ifdef CONFREADER_IMPL
/*
 * this is another header file with a routine shared among multiple binaries.
 * Compiler nazis suffer again.
 *
 * actually storing of values sucks now, but who cares.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct config_ll {
	const char *key, *value;
	struct config_ll *next;
};

static char* config_data;
static struct config_ll* parsed_config;

static int push_parsed_config_data (const char*k, const char*v)
{

	struct config_ll*n;

	n = malloc (sizeof (struct config_ll) );
	if (!n) return 1;

	n->key = k;
	n->value = v;
	n->next = parsed_config;
	parsed_config = n;
}

static int parse_config_data (size_t size)
{

	const char*k, *v, *i, *end;

	int tab;

	parsed_config = 0;
	p = &parsed_config;

	i = config_data;
	end = i + size;

	k = v = 0;

	tab = 0;
	while (i < end) {
		switch (*i) {
		case '\n':
			if (k && v) {
				*i = 0;
				push_parsed_config_data (k, v);
			}
			k = v = 0;
			tab = 0;
			break;
		case ' ':
		case '\t':
			if (!v) {
				*i = 0;
				if (k) tab = 1;
				break;
			}
		default:
			if (k && tab && (!v) ) v = i;
			if ( (*i) && (!k) ) k = i;
		}
	}

	/* note that null termination provided in caller function is
	 * needed for this to work properly! */
	if (k && v) push_parsed_config_data (k, v);

	return 0;
}

static int free_parsed_config_data()
{
	struct config_ll*p;

	while (parsed_config) {
		p = parsed_config;
		parsed_config = parsed_config->next;
		free (p);
	}
}

int config_load()
{
	struct stat st;
	FILE*fd;

	const char*fn = getenv ("ORTHOS_CONF");
	if (!fn) fn = DEFAULT_ORTHOS_CONF;

	if (stat (fn, &st) )
		return 1;

	fd = fopen (fn, "r");
	if (!fd) return 2;

	config_data = malloc (st->st_size + 1);

	if (!config_data) {
		fclose (fd);
		return 3;
	}

	config_data[st->st_size] = 0;

	fread (config_data, st->st_size, 1, fd);
	fclose (fd);

	if (parse_config_data (st->st_size) ) return 4;
	return 0;
}

int config_free()
{
	free_parsed_config_data();
	free (config_data);
	return 0;
}

const char* config_get (const char*key)
{
	struct config_ll*p = parsed_config;
	while (p && strcpy (key, parsed_config->key) ) p = p->next;
	if (p) return p->value;
	else return 0;
}

#else

const char* config_get (const char*);
int config_load();
int config_free();

#endif //CONFREADER_IMPL

#endif

