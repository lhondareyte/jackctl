PRG     = jackctl
SRCS    = jackctl.c jackcmds.c
OBJS    = jackctl.o jackcmds.o 

CFLAGS  = -I/usr/include -I/usr/local/include -Werror -pedantic
LDFLAGS = -L/usr/local/lib -L/usr/lib -ljack
BINDIR  = $(DESTDIR)$(PREFIX)/bin
MANDIR  = $(DESTDIR)$(PREFIX)/share/man/man1

all: $(OBJS)
	$(CC) $(OBJS) -o $(PRG) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

install:
	install -m 755 $(PRG) $(BINDIR)
	gzip -cn $(PRG).1 > $(PRG).1.gz
	install -m 644 $(PRG).1.gz $(MANDIR)

man:
	go-md2man < $(PRG).md > $(PRG).1

clean:
	rm -f $(OBJS) $(PRG)

