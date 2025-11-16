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

int usage(void){
	fprintf(stderr, "Usage:\n  jackctl [-l] [-C] [-D] [-c source destination] [-d source destination]\n");
	exit(1);
}

int main(int argc, char *argv[]) {

	int action = UNKNOWN;
	int opt;
	int rc = 0;

	/* Checking command line options */
	if (argc > 1) {
		while ((opt = getopt (argc, argv, "c:d:f:lCD")) != -1 ) {
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
			default:
				usage();
				break;
			}
		}
	}
	if (optind < 2) {
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
		return 1;
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
			rc = disconnect_all();
			run_config(argv[optind - 1]);
			break;
		case CONNECT:
			rc = connect_ports (argv[optind], argv[optind + 1]);
			break;
		case DISCONNECT:
			rc = disconnect_ports (argv[optind], argv[optind + 1], 1);
			break;
	}
	/* Cleanup */
	jack_client_close(client);
	if ( rc > 0) rc = 1;
	return rc;
}
