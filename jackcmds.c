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

#ifndef TRUE
#define TRUE 1
#endif

extern jack_client_t *client;

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
		fprintf(stderr, "Error: cannot connect %s to %s\n", src, dst);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int disconnect_ports(const char *src, const char *dst) {
	if (jack_disconnect(client, src, dst)) {
		fprintf(stderr, "Error: cannot disconnect %s from %s\n", src, dst);
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

