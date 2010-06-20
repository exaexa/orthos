
#ifndef ORTHOS_CONFLOADER_H
#define ORTHOS_CONFLOADER_H

#ifndef DEFAULT_ORTHOS_CONF
#define DEFAULT_ORTHOS_CONF "/etc/orthos.conf"
#endif

#ifdef CONFLOADER_IMPL
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

static char* config_data = 0;
static struct config_ll* parsed_config = 0;

static int push_parsed_config_data (const char*k, const char*v)
{
	struct config_ll*n;

	n = malloc (sizeof (struct config_ll) );
	if (!n) return 1;

	n->key = k;
	n->value = v;
	n->next = parsed_config;
	parsed_config = n;

	return 0;
}

static void dump_config()
{
	struct config_ll*p = parsed_config;
	while (p) {
		printf ("`%s' -> `%s'\n", p->key, p->value);
		p = p->next;
	}
}

static int parse_config_data (size_t size)
{

	char*k, *v, *i, *end;

	int tab;

	parsed_config = 0;

	i = config_data;
	end = i + size;

	k = v = 0;

	tab = 0;
	while (i < end) {
		switch (*i) {
		case '\n':
			if (k && v) {
				*i = 0;
				if(push_parsed_config_data (k, v))
					return 1;
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
		++i;
	}

	/* note that null termination provided in caller function is
	 * needed for this to work properly! */
	if (k && v) if (push_parsed_config_data (k, v)) return 1;

	/* DEBUG */
	dump_config();

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
	return 0;
}

int config_load()
{
	struct stat st;
	const char*fn;
	FILE*fd;

	fn = getenv ("ORTHOS_CONF");
	if (!fn) fn = DEFAULT_ORTHOS_CONF;

	if (stat (fn, &st) )
		return 1;

	fd = fopen (fn, "rb");
	if (!fd) return 2;

	config_data = malloc (st.st_size + 1);

	if (!config_data) {
		fclose (fd);
		return 3;
	}

	config_data[st.st_size] = 0;

	fread (config_data, st.st_size, 1, fd);
	fclose (fd);

	if (parse_config_data (st.st_size) ) return 4;
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
	while (p && strcmp (key, p->key) ) p = p->next;
	if (p) return p->value;
	else return 0;
}

#else

const char* config_get (const char*);
int config_load();
int config_free();

#endif //CONFLOADER_IMPL

#endif

