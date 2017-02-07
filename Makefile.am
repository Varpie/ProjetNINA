AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = nina
nina_SOURCES = \
    src/main.cpp \
    src/navigation/navigator.cpp \
    src/navigation/intelligence.cpp \
    src/ctrl_keyboard/timed_keystrokes.c \
    src/ctrl_keyboard/write_keyboard.c
AM_LDFLAGS=-lpython2.7 -lX11 -lpthread
