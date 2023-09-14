TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        loader.cpp \
        main.cpp

HEADERS += \
    loader.h
LIBS += -lcurl
