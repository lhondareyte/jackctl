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

void list_connections(jack_client_t *client) {
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
		return -1;
	}
	return 0;
}

int disconnect_ports(const char *src, const char *dst, int verbose) {
	if (jack_disconnect(client, src, dst)) {
		if (verbose != 0) {
			fprintf(stderr, "Error: cannot disconnect %s from %s\n", src, dst);
			return -1;
		}
	}
	return 0;
}

void disconnect_all(jack_client_t *client) {
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
				disconnect_ports(ports[i], connections[j], 0);
            }
            jack_free(connections);
        } 
    }
    jack_free(ports);

}
