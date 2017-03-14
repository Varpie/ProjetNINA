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

bindeptsdir = \
	$(prefix)/bin

pythonlib_DATA = \
	src/navigation/nina.py

bindepts_DATA = \
	geckodriver

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
	mkdir $(DESTDIR)$(sysconfdir); \
	mkdir $(DESTDIR)$(sysconfdir)/nina ; \
	cd $(CURRENT_DIR)/src/ctrl_keyboard && \
	$(PYTHON) setup.py install \
		--root $(DESTDIR) \
		--record $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt \
		--verbose && \
	cd -; \
	echo $(prefix)/bin/geckodriver >> $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt;

uninstall-local:
	cat $(DESTDIR)$(sysconfdir)/nina/nina_installed_files.txt | xargs rm -rf;
	rm $(DESTDIR)$(pythondir)/nina.*; \
	rm -r $(DESTDIR)$(sysconfdir)/nina/;
