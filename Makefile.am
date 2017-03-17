AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = nina
ninadir = .
SUBDIR = src doc scripts debian

#target to create kernel object
src/module_kernel/ninartk.ko:
	cd $(CURRENT_DIR)/src/module_kernel/ && $(MAKE)

nina_SOURCES = \
	src/main.cpp \
	src/navigation/navigator.cpp \
	src/navigation/intelligence.cpp \
	src/ctrl_keyboard/timed_keystrokes.c \
	src/ctrl_keyboard/write_keyboard.c \
	src/ctrl_keyboard/manage_devices.c

nina_HEADERS = \
	src/main.hpp \
	src/navigation/navigator.hpp \
	src/navigation/intelligence.hpp \
	src/ctrl_keyboard/write_keyboard.h \
	src/lib.hpp

ninaconfdir= \
	$(localstatedir)/nina

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

ninartkconfdir = \
	/lib/modprobe.d/

ninartkconf_DATA = \
	config/ninartk.conf

ninartkdir = \
	/lib/modules/$(UNAME)/misc/

ninartk_DATA = \
	src/module_kernel/ninartk.ko

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

CONFPATH=\"$(localstatedir)/nina/\"
CPPFLAGS = -DCONFPATH=$(CONFPATH)

AM_LDFLAGS=-lpython2.7 -lX11 -lpthread -lXi

dist_man1_MANS = doc/nina.1

dist_doc_DATA = \
	README.md \
	COPYING

install-exec-local:
	mkdir -p $(DESTDIR)$(localstatedir); \
	mkdir -p $(DESTDIR)$(localstatedir)/nina ; \
	cd $(CURRENT_DIR)/src/ctrl_keyboard && \
	$(PYTHON) setup.py install \
		--root $(DESTDIR) \
		--record $(DESTDIR)$(localstatedir)/nina/nina_installed_files.txt \
		--verbose && \
	cd -; \
	./scripts/python_dependencies.sh; \
	echo $(DESTDIR)$(prefix)/bin/geckodriver >> $(DESTDIR)$(localstatedir)/nina/nina_installed_files.txt; \
	echo $(DESTDIR)/lib/modprobe.d/ninartk.conf >> $(DESTDIR)$(localstatedir)/nina/nina_installed_files.txt; \
	echo $(DESTDIR)/lib/modules/$(UNAME)/misc/ninartk.ko >> $(DESTDIR)$(localstatedir)/nina/nina_installed_files.txt;


install-data-hook:
	chmod +x $(DESTDIR)$(prefix)/bin/geckodriver;

uninstall-local:
	cat $(DESTDIR)$(localstatedir)/nina/nina_installed_files.txt | xargs rm -rf; \
	rm $(DESTDIR)$(pythondir)/nina.*; \
	rm -r $(DESTDIR)$(localstatedir)/nina/;

#important, else it's called by dh_clean when .deb and packaging fails
clean-comp:
	make clean; \
	rm -rf \
		Makefile Makefile.in aclocal.m4 autom4te.cache/ compile config.log config.status \
		configure debian/.debhelper/ debian/debhelper-build-stamp debian/files debian/nina./ \
		debian/nina.debhelper.log debian/nina.substvars debian/nina/ depcomp geckodriver \
		install-sh missing src/.deps/ src/.dirstamp src/ctrl_keyboard/.deps/ src/ctrl_keyboard/.dirstamp \
		src/ctrl_keyboard/build/ src/navigation/.deps/ src/navigation/.dirstamp geckodriver.log;
