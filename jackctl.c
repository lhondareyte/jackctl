/*
 Copyright (C) 2025 Luc Hondareyte

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jack/jack.h>

#include "jackctl.h"

jack_client_t *client;

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

int connect_ports(const char *src, const char *dst) {
	if (jack_connect(client, src, dst)) {
		fprintf(stderr, "Error: cannot connect %s to %s\n", src, dst);
		return -1;
	}
	return 0;
}

int disconnect_ports(const char *src, const char *dst) {
	if (jack_disconnect(client, src, dst)) {
		fprintf(stderr, "Error: cannot disconnect %s from %s\n", src, dst);
		return -1;
	}
	return 0;
}

int usage(void){
	// fprintf(stderr, "Usage: programme [-l] [-C] [-f config] [-c source destination] [-d source destination]\n");
	fprintf(stderr, "Usage:\n  jackctl [-l] [-c source destination] [-d source destination]\n");
    exit(1);
}

int main(int argc, char *argv[]) {

	int action = UNKNOWN;
	int opt;

	int rc=0;

	//char config[256];

	/* Checking command line options */
	if (argc > 1) {
		while ((opt = getopt (argc, argv, "c:d:f:lC")) != -1 ) {
			switch (opt) {
			case 'l': 
				action = LIST_PORTS;
				break;
			case 'C':
				action = LIST_CONNECTIONS;
				break;
			case 'f': 
				action = RUN_CONFIG;
				break;
			case 'c':
				action = CONNECT;
				break;
			case'd':
				action = DISCONNECT;
				break;
			default:
				usage();
				break;
			}
		}
	}
	else {
		usage();
	}
	if (action == CONNECT || action == DISCONNECT) {
		if (optind + 1 != argc){
			usage();
		}
		optind--;
	}
	/* Jack client initialization */
	client = jack_client_open("jack_connect", JackNullOption, NULL);
	if (client == NULL) {
		fprintf(stderr, "Erreur: impossible de se connecter au serveur JACK\n");
		return 1;
	}

	if (jack_activate(client)) {
		fprintf(stderr, "Erreur: impossible d'activer le client JACK\n");
		jack_client_close(client);
		return 1;
	}
	
	/* Perform differents actions */
	switch (action) {
		case LIST_PORTS:
			list_ports();
			break;
		case LIST_CONNECTIONS:
			break;
		case RUN_CONFIG:
			break;
		case CONNECT:
			rc = connect_ports (argv[optind], argv[optind + 1]);
			break;
		case DISCONNECT:
			rc = disconnect_ports (argv[optind], argv[optind + 1]);
			break;
	}
	/* Cleanup */
	jack_deactivate(client);
	jack_client_close(client);
	return rc;
}
