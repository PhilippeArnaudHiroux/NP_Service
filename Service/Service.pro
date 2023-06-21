TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lzmq -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += \
        commands.cpp \
        main.cpp \
        shops.cpp \
        split.cpp \
        txtfile.cpp \
        zmqserver.cpp

HEADERS += \
    commands.h \
    shops.h \
    split.h \
    txtfile.h \
    zmqserver.h
