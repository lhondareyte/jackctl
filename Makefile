PRG     = jackctl
SRCS    = jackctl.c
OBJS    = jackctl.o
CFLAGS  = -I/usr/include -I/usr/local/include -Werror
LDFLAGS = -L/usr/local/lib -L/usr/lib -ljack

all: $(OBJS)
	$(CC) $(OBJS) -o $(PRG) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PRG)

