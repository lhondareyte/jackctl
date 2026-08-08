/*
 * Copyright (C) 2025-2026 Luc Hondareyte
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef __JACKCTL_H__
#define __JACKCTL_H__

#include <stdbool.h>

int connect_ports(const char *, const char *);
int disconnect_ports(const char *, const char *);
int disconnect_all(void);
int parse_config(char *);
int run_config(void);

void list_connections(void);
void list_ports(void);
 
#define	UNKNOWN           0x00
#define	LIST_PORTS        0x01 
#define	LIST_CONNECTIONS  0x02
#define	RUN_CONFIG        0x03
#define	CONNECT           0x04
#define	DISCONNECT        0x05
#define	DISCONNECT_ALL    0x06
#define	CONNECT_ALL       0x07
#define	CONNECT_ALL_MIDI  0x08
#define	CONNECT_ALL_AUDIO 0x09

#endif /* __JACKCTL_H__ */
