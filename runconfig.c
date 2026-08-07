/*
 * Copyright (C) 2025-2026 Luc Hondareyte
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jack/jack.h>

#define LINE_LENGTH 256
#define MAX_LINES   128

#ifndef TRUE
#define TRUE 1
#endif

extern jack_client_t *client;

extern int connect_ports(const char *, const char *);
typedef struct {
	char *section[LINE_LENGTH];
	char *source[LINE_LENGTH];
	char *destination[LINE_LENGTH];
	int s_count;   /* sections count */
	int l_count;   /* lines count */
} Config;

Config config = { 
	.s_count = 0,
	.l_count = 0
};

void trim(char *str) {
	char *p = str;
	while(isspace(*p)) {
		p++;
	}
	memmove(str, p, strlen(p) + 1);
	p = str + strlen(str) - 1;
	while(p > str && isspace(*p)) {
		*p-- = '\0';
	}
}

int parse_config (char * filename) {

	char line[LINE_LENGTH];

	FILE *fp;

	int len = 0; /* line length */
	int rc = EXIT_SUCCESS;
	int i = 0;

	char *section = NULL;

	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "Error: cannot open %s.\n", filename);
		return EXIT_FAILURE;
	}

	/* Load config file */
	while (fgets(line, LINE_LENGTH, fp) && config.l_count < MAX_LINES) {

		config.l_count++;

		/* Ignore empty lines or comments */
		if (line[0] == '*' || line[0] == '#' || line[0] == ';' || line[0] == '\n') {
			continue;
		}

		trim(line);
		len = strlen(line);

		if (len > 0) {
			if (line[0] == '[' && line[len - 1] == ']') {
				if (len > 2) {
					section = strdup(line);
					/* remove bracket */
					for (int j = 0; j < len -2; j++ ) {
						section[j] = section[j + 1];
					}
					section[len - 2] = '\0';
					config.section[config.s_count] = strdup(section);
					
					config.s_count++;
					continue;
				}
				else {
					fprintf(stderr, "Error: %s[%d]: Invalid section name\n", filename, config.l_count);
					rc = EXIT_FAILURE;
					goto exit_parse;
				}
			}
			/* Get key/value pairs */
			char *equal_sign = strchr(line, '=');
			if (equal_sign) {
				*equal_sign = '\0';
				char *key = line;
				char *value = equal_sign + 1;
				trim(key);
				trim(value);

				if (strcmp(key, "source") == 0) {
					if (strlen(value) > 0) {
						config.source[config.s_count - 1 ] = strdup(value);
					}
				}
				else if (strcmp(key, "destination") == 0) {
					if (strlen(value) > 0) {
						config.destination[config.s_count - 1] = strdup(value);
					}
				}
				else {
					fprintf(stderr, "Error: %s[%d]: Unknown action \"%s\"\n", filename, config.l_count, key);
					rc = EXIT_FAILURE;
					goto exit_parse;
				}
			}
		}
	}

exit_parse:
	free(section);
	if (rc != 0)
		rc = EXIT_FAILURE;
	return rc;
}

int run_config(void) {		
	int rc = EXIT_SUCCESS;

	for (int i = 0; i < config.s_count; i++) {
		if (config.section[i] && config.source[i] && config.destination[i]) {
					rc += connect_ports(config.source[i], config.destination[i]);
		}
		else if(! config.source[i]) {
			fprintf(stderr, "Error: Missing source in \"%s\" section\n", config.section[i]);
			rc = EXIT_FAILURE;
		}
		else if(! config.destination[i]) {
			fprintf(stderr, "Error: Missing destination in \"%s\" section\n", config.section[i]);
			rc = EXIT_FAILURE;
		}
	}
	if (rc > 0)
		rc = EXIT_FAILURE;
	return rc;
}

