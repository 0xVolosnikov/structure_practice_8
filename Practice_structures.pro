
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
QMAKE_LFLAGS += -lncurses
QMAKE_LFLAGS_CONSOLE += -lncurses

