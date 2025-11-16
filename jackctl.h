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
#ifndef __JACKCTL_H__

void list_ports(void);
int connect_ports(const char *, const char *);
int disconnect_ports(const char *, const char *, int);
void list_connections(void);
int disconnect_all(void);
int run_config(char *);

#define	UNKNOWN          0
#define	LIST_PORTS       1
#define	LIST_CONNECTIONS 2
#define	RUN_CONFIG       3
#define	CONNECT          4
#define	DISCONNECT       5
#define	DISCONNECT_ALL   6

#endif /* __JACKCTL_H__ */
