PRG     = jackctl
SRCS    = jackctl.c jackcmds.c
OBJS    = jackctl.o jackcmds.o 

CFLAGS  = -I/usr/include -I/usr/local/include -Werror
LDFLAGS = -L/usr/local/lib -L/usr/lib -ljack

all: $(OBJS)
	$(CC) $(OBJS) -o $(PRG) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

man:
	go-md2man < $(PRG).md > $(PRG).1
clean:
	rm -f $(OBJS) $(PRG) *.1

