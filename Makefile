prefix = /usr/local

all:
	gcc ./bin/des.c -o des

install:
	gcc ./bin/des.c -o des
	echo "DESTDIR: $(DESTDIR)"
	echo "prefix: $(prefix)"
	install ./des $(DESTDIR)$(prefix)/bin
	install -m 644 des.1 $(DESTDIR)$(prefix)/share/man/man1/des.1

clean:
	rm -f des
