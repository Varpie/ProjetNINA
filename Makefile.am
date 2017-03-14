AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = nina
ninadir = .
SUBDIR = src doc scripts debian

nina_SOURCES = \
	src/main.cpp \
	src/navigation/navigator.cpp \
	src/navigation/intelligence.cpp \
	src/ctrl_keyboard/timed_keystrokes.c \
	src/ctrl_keyboard/write_keyboard.c

nina_HEADERS = \
	src/main.hpp \
	src/navigation/navigator.hpp \
	src/navigation/intelligence.hpp \
	src/ctrl_keyboard/timed_keystrokes.h \
	src/ctrl_keyboard/write_keyboard.h \
	src/log.hpp

ninaconfdir= \
	$(sysconfdir)/nina

ninaconf_DATA = \
	config/dictionaries/auto_blacklist.txt \
	config/dictionaries/blacklist.txt \
	config/dictionaries/keywords.txt \
	config/dictionaries/rubbish_links.txt \
	config/dictionaries/whitelist.txt

pythonlibdir = \
	$(pythondir)

pythonlib_DATA = \
	src/navigation/nina.py

bindepts_DATA = \
	geckodriver

bindeptsdir = \
	$(prefix)/bin

EXTRA_DIST = \
	scripts/get_geckodriver.sh \
	src/navigation/nina.py \
	src/ctrl_keyboard/uinput_wrapping_module.c \
	src/ctrl_keyboard/setup.py \
	config/dictionaries/auto_blacklist.txt \
	config/dictionaries/blacklist.txt \
	config/dictionaries/keywords.txt \
	config/dictionaries/rubbish_links.txt \
	config/dictionaries/whitelist.txt

AM_LDFLAGS=-lpython2.7 -lX11 -lpthread -lXi

dist_man1_MANS = doc/nina.1

dist_doc_DATA = \
	README.md \
	COPYING

install-exec-local:
	mkdir -p $(DESTDIR)$(sysconfdir); \
	mkdir -p $(DESTDIR)$(sysconfdir)/nina ; \
	cd $(CURRENT_DIR)/src/ctrl_keyboard && \
	$(PYTHON) setup.py install \
		--root $(DESTDIR) \
		--record $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt \
		--verbose && \
	cd -; \
	echo $(DESTDIR)$(prefix)/bin/geckodriver >> $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt; \
	./scripts/python_dependencies.sh;


install-data-hook:
	chmod +x $(DESTDIR)$(prefix)/bin/geckodriver;

uninstall-local:
	cat $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt | xargs rm -rf; \
	rm $(DESTDIR)$(pythondir)/nina.*; \
	rm -r $(DESTDIR)$(sysconfdir)/nina/;

clean-local:
	rm -rf \
		Makefile Makefile.in aclocal.m4 autom4te.cache/ compile config.log config.status \
		configure debian/.debhelper/ debian/debhelper-build-stamp debian/files debian/nina./ \
		debian/nina.debhelper.log debian/nina.substvars debian/nina/ depcomp geckodriver \
		install-sh missing src/.deps/ src/.dirstamp src/ctrl_keyboard/.deps/ src/ctrl_keyboard/.dirstamp \
		src/ctrl_keyboard/build/ src/navigation/.deps/ src/navigation/.dirstamp geckodriver.log;
