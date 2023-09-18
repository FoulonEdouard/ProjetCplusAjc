TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        convertepoch.cpp \
        loader.cpp \
        main.cpp

HEADERS += \
    convertepoch.h \
    loader.h

LIBS += -lcurl
LIBS += -lgd -L/usr/include/gd.h



