prefix = /usr/local

all:
	gcc ./bin/des.cpp -o des -lcurl -lcrypto -std=c++17

install:
	gcc ./bin/des.cpp -o des -lcurl -lcrypto -std=c++17
	echo "DESTDIR: $(DESTDIR)"
	echo "prefix: $(prefix)"
	install des $(DESTDIR)$(prefix)/bin
	install -m 644 des.1 $(DESTDIR)$(prefix)/share/man/man1/des.1
	install -m 755 -d $(DESTDIR)$(prefix)/share/man/man2
	install -m 644 des.2 $(DESTDIR)$(prefix)/share/man/man2/des.2

clean:
	rm -f des
