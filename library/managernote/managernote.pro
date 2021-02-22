#-------------------------------------------------
#
# Project created by QtCreator 2019-08-20T13:59:15
#
#-------------------------------------------------

QT       += sql

#QT       -= gui

TARGET = managernote
TEMPLATE = lib
CONFIG += staticlib c++17 create_prl

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
        ManagersNote.cpp \
    EntityNote.cpp \
    BddNote.cpp \
    ConfigNote.cpp

HEADERS += \
        ManagersNote.h \
    EntityNote.h \
    BddNote.h \
    ConfigNote.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/entities/release/ -lentities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/entities/debug/ -lentities
else:unix: LIBS += -L$$PWD/../../../compilation/lib2d/entities/ -lentities

INCLUDEPATH += $$PWD/../../../lib2d/entities
DEPENDPATH += $$PWD/../../../lib2d/entities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/entities/release/libentities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/entities/debug/libentities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/entities/release/entities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/entities/debug/entities.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/entities/libentities.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/manager/release/ -lmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/manager/debug/ -lmanager
else:unix: LIBS += -L$$PWD/../../../compilation/lib2d/manager/ -lmanager

INCLUDEPATH += $$PWD/../../../lib2d/manager
DEPENDPATH += $$PWD/../../../lib2d/manager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/manager/release/libmanager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/manager/debug/libmanager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/manager/release/manager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/manager/debug/manager.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/manager/libmanager.a
