CFLAGS = -Wall -pedantic

all: sense

clean:
	- rm -rf *.o sense

install: all
	install -d $(DESTDIR)/usr/bin/
	install -o 0 -g 0 -m 0755 sense $(DESTDIR)/usr/bin/
