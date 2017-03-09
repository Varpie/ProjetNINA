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
	src/ctrl_keyboard/conf \
	config/dictionaries/auto_blacklist.txt \
	config/dictionaries/blacklist.txt \
	config/dictionaries/keywords.txt \
	config/dictionaries/rubbish_links.txt \
	config/dictionaries/whitelist.txt

pythonlibdir = \
	$(pythondir)

pythonlib_DATA = \
	src/navigation/nina.py

EXTRA_DIST = \
	scripts/get_geckodriver.sh \
	src/navigation/nina.py \
	src/ctrl_keyboard/uinput_wrapping_module.c \
	src/ctrl_keyboard/setup.py \
	src/ctrl_keyboard/conf \
	config/dictionaries/auto_blacklist.txt \
	config/dictionaries/blacklist.txt \
	config/dictionaries/keywords.txt \
	config/dictionaries/rubbish_links.txt \
	config/dictionaries/whitelist.txt \
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/rules \
	debian/sources/format

AM_LDFLAGS=-lpython2.7 -lX11 -lpthread -lXi
dist_man1_MANS = doc/nina.1

install-exec-local:
	cd $(CURRENT_DIR)/src/ctrl_keyboard && \
	$(PYTHON) setup.py install \
	--verbose \
	--prefix $(DESTDIR)$(prefix) \
	--record $(DESTDIR)$(pythondir)/nina_py_installed_files.txt && \
	cd -; \
	sh $(CURRENT_DIR)/scripts/get_geckodriver.sh;
#cd $(CURRENT_DIR)/src/module_kernel && \
#make && \
#cp ninartk.ko /lib/modules/$(uname -r)/misc && \
#modprobe ninartk;

uninstall-local:
	( cat $(DESTDIR)$(pythondir)/nina_py_installed_files.txt | xargs rm -rf \
	rm $(DESTDIR)$(pythondir)/nina* \
	rm -r $(DESTDIR)$(sysconfdir)/nina )