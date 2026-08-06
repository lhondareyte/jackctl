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

#define MAX_LINE 256
#define MAX_CONFIG 1000

#ifndef TRUE
#define TRUE 1
#endif

extern jack_client_t *client;
extern bool verbose;

typedef struct {
	char *lines[MAX_CONFIG];
	int  lines_number[MAX_CONFIG];
	int  count;
} Config;

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

void list_ports(void) {
	const char **ports;
	const char **port;

	/* Get AUDIO input ports */
	ports = jack_get_ports(client, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput);
	if (ports) {
		for (port = ports; *port; port++) {
			printf("playback: %s\n", *port);
		}
		free(ports); 
	}

	/* Get AUDIO output ports */
	ports = jack_get_ports(client, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
	if (ports) {
		for (port = ports; *port; port++) {
			printf("capture: %s\n", *port);
		}
		free(ports); 
	}

	/* Get MIDI input ports */
	ports = jack_get_ports(client, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsInput);
	if (ports) {
		for (port = ports; *port; port++) {
			printf("midi_in: %s\n", *port);
		}
		free(ports); 
	}

	/* Get MIDI output ports  */
	ports = jack_get_ports(client, NULL, JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput);
	if (ports) {
		for (port = ports; *port; port++) {
			printf("midi_out: %s\n", *port);
		}
		free(ports); 
	}
}

void list_connections(void) {
	const char **ports, **connections;
	int i, j;

	ports = jack_get_ports(client, NULL, NULL, 0);
	if (ports == NULL) {
		return;
	}

	/* Get connexions for each port */
	for (i = 0; ports[i] != NULL; i++) {
		connections = jack_port_get_all_connections(client, jack_port_by_name(client, ports[i]));
		if (connections != NULL) {
			for (j = 0; connections[j] != NULL; j++) {
				fprintf(stdout, "%s -> %s\n", ports[i], connections[j]);
			}
			jack_free(connections);
		} 
	}
	jack_free(ports);
}

int connect_ports(const char *src, const char *dst) {
	if (jack_connect(client, src, dst)) {
		if (verbose == TRUE) {
			fprintf(stderr, "Error: cannot connect %s to %s\n", src, dst);
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int disconnect_ports(const char *src, const char *dst) {
	if (jack_disconnect(client, src, dst)) {
		if (verbose == TRUE) {
			fprintf(stderr, "Error: cannot disconnect %s from %s\n", src, dst);
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int disconnect_all(void) {
	const char **ports, **connections;
	int i, j;
	int rc = EXIT_SUCCESS;

	ports = jack_get_ports(client, NULL, NULL, 0);
	if (ports == NULL) {
		return EXIT_SUCCESS;
	}

	/* Get connexions for each port */
	for (i = 0; ports[i] != NULL; i++) {
		connections = jack_port_get_all_connections(client, jack_port_by_name(client, ports[i]));
		if (connections != NULL) {
			for (j = 0; connections[j] != NULL; j++) {
				rc += disconnect_ports(ports[i], connections[j]);
			}
			jack_free(connections);
		} 
	}
	jack_free(ports);
	if (rc != 0)
		rc = EXIT_FAILURE;
	return rc;
}

int run_config(char * filename) {

	char *section = NULL;
	char *source = NULL;
	char *destination = NULL;
	char *action = NULL;
	char line[MAX_LINE];

	Config config = { .count = 0 };
	FILE *fp;

	int len = 0; /* line length */
	int rc = EXIT_SUCCESS;
	int i = 0;

	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "Error: cannot open %s!\n", filename);
		return EXIT_FAILURE;
	}

	disconnect_all();

	/* Load config file */
	while (fgets(line, MAX_LINE, fp) && config.count < MAX_CONFIG) {
		i++;
		config.lines_number[config.count] = i;
		trim(line);
		if (strlen(line) > 0) {
			config.lines[config.count] = strdup(line);
			config.count++;
		}
	}
	fclose(fp);

	for (i = 0; i < config.count; i++) {
		char *current_line = config.lines[i];

		/* Ignore empty lines or comments */
		if (current_line[0] == '*' || current_line[0] == '#' || current_line[0] == ';') {
			continue;
		}

		/*  Sections */
		if (current_line[0] == '[' && current_line[strlen(current_line) - 1] == ']') {
			if (strlen(current_line) > 2) {
				free(action); action = NULL;
				free(source); source = NULL;
				free(destination); destination = NULL;
				len = strlen(current_line);
				section = strdup(current_line);
				for (int j = 0; j < len -2; j++ ) {
					section[j] = section[j + 1];
				}
				section[len - 2] = '\0';
				
				continue;
			}
			else {
				fprintf(stderr, "Error: %s[%d]: Invalid section name\n", filename, config.lines_number[i]);
				rc = EXIT_FAILURE;
				goto cleanup_config;
			}
		}

		/* Get key/value pairs */
		char *equal_sign = strchr(current_line, '=');
		if (equal_sign) {
			*equal_sign = '\0';
			char *key = current_line;
			char *value = equal_sign + 1;
			trim(key);
			trim(value);

			if (strcmp(key, "action") == 0) {
				action = strdup(value);
			}
			else if (strcmp(key, "source") == 0) {
				source = strdup(value);
			}
			else if (strcmp(key, "destination") == 0) {
				destination = strdup(value);
			}
			else {
				fprintf(stderr, "Error: %s[%d]: Unknown action \"%s\"\n", filename, config.lines_number[i], key);
				rc = EXIT_FAILURE;
				goto cleanup_config;
			}
		}

		if (action && source && destination) {
			if ((strcmp(action, "connect")) == 0) {
				rc += connect_ports(source, destination);
			}
			else if ((strcmp(action, "disconnect")) == 0) {
				rc += disconnect_ports(source, destination);
			}
		}
	}

cleanup_config:
	/* Cleanup */
	for (int i = 0; i < config.count; i++) {
		free(config.lines[i]);
	}
	free(section);
	free(action);
	free(source);
	free(destination);
	if (rc != 0)
		rc = EXIT_FAILURE;
	return rc;
}

