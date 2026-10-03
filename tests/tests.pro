TEMPLATE = app
TARGET = tests
QT += qml quick testlib
CONFIG += console c++11 automoc
LIBS += -L../qmlchewing -lqmlchewing -lchewing

# Input

INCLUDEPATH += ../qmlchewing/inc

SOURCES += \
    test_chewing.cpp

HEADERS += \
    ../qmlchewing/inc/qmlchewing_plugin.h \
    ../qmlchewing/inc/chewing.h

