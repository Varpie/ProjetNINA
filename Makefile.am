AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = nina
ninadir = .

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

EXTRA_DIST = \
  config/conf \
  config/nina.conf \
  config/dictionaries/auto_blacklist.txt \
  config/dictionaries/blacklist.txt \
  config/dictionaries/keywords.txt \
  config/dictionaries/rubbish_links.txt \
  config/dictionaries/whitelist.txt \
  scripts/python_dependencies.sh \
  src/ctrl_keyboard/uinput_wrapping_module.c \
  src/ctrl_keyboard/setup.py \
  scripts/setup_python.sh \
  src/navigation/nina.py

AM_LDFLAGS=-lpython2.7 -lX11 -lpthread
dist_man1_MANS = doc/nina.1
