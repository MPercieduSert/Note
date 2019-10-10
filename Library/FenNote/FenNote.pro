#-------------------------------------------------
#
# Project created by QtCreator 2019-08-20T13:56:43
#
#-------------------------------------------------

QT       += widgets sql

TARGET = FenNote
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FenPrincipaleNote.cpp \
    NoyauNote.cpp \
    TabNote.cpp \
    ZoneCentraleNote.cpp

HEADERS += \
    FenPrincipaleNote.h \
    NoyauNote.h \
    TabNote.h \
    ZoneCentraleNote.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/mps/release/ -lmps
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/mps/debug/ -lmps
else:unix: LIBS += -L$$PWD/../../../../Compilation/libmps/mps/ -lmps

INCLUDEPATH += $$PWD/../../../libmps
DEPENDPATH += $$PWD/../../../libmps

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/mps/release/libmps.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/mps/debug/libmps.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/mps/release/mps.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/mps/debug/mps.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/mps/libmps.a
