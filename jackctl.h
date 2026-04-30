/*
 * Copyright (C) 2025 Luc Hondareyte
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef __JACKCTL_H__

#include <stdbool.h>

int connect_ports(const char *, const char *);
int disconnect_ports(const char *, const char *);
int disconnect_all(void);
int run_config(char *);
void list_ports(void);
void list_connections(void);

#define	UNKNOWN          0
#define	LIST_PORTS       1
#define	LIST_CONNECTIONS 2
#define	RUN_CONFIG       3
#define	CONNECT          4
#define	DISCONNECT       5
#define	DISCONNECT_ALL   6

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

bool verbose = FALSE;

#endif /* __JACKCTL_H__ */
