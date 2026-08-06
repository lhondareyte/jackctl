/*
 * Copyright (C) 2025-2026 Luc Hondareyte
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jack/jack.h>

#include "jackctl.h"

jack_client_t *client;

int usage(void){
	fprintf(stderr, "Usage:\n  jackctl [-l] | [-C] | [-D] | -f <config.ini> | [-c src dst] [-d src dst] | [-v]\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

	int action = UNKNOWN;
	int opt;
	int rc = 0;

	/* Checking command line options */
	if (argc > 1) {
		while ((opt = getopt (argc, argv, "c:d:f:lCDv")) != -1 ) {
			switch (opt) {
			case 'l': 
				action = LIST_PORTS;
				break;
			case 'C':
				action = LIST_CONNECTIONS;
				break;
			case 'D':
				action = DISCONNECT_ALL;
				break;
			case 'f': 
				action = RUN_CONFIG;
				break;
			case 'c':
				action = CONNECT;
				break;
			case 'd':
				action = DISCONNECT;
				break;
			case 'v':
				verbose = TRUE;
				break;
			default:
				usage();
				break;
			}
		}
	}
	if ((optind < 2) || (action == UNKNOWN)) {
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
		fprintf(stderr, "Error: Unable to connect to JACK server\n");
		return EXIT_FAILURE;
	}

	/* Perform differents actions */
	switch (action) {
	case LIST_PORTS:
		list_ports();
		break;
	case DISCONNECT_ALL:
		rc = disconnect_all();
		break;
	case LIST_CONNECTIONS:
		list_connections();
		break;
	case RUN_CONFIG:
		rc = run_config(argv[optind - 1]);
		break;
	case CONNECT:
		rc = connect_ports (argv[optind], argv[optind + 1]);
		break;
	case DISCONNECT:
		rc = disconnect_ports (argv[optind], argv[optind + 1]);
		break;
	}
	/* Cleanup */
	jack_client_close(client);
	return rc;
}
