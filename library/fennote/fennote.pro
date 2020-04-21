#-------------------------------------------------
#
# Project created by QtCreator 2019-08-20T13:56:43
#
#-------------------------------------------------

QT       += widgets sql

TARGET = fennote
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
    FenPrincipaleNote.cpp \
    NoyauNote.cpp \
    TabNote.cpp \
    ZoneCentraleNote.cpp \
    ElevesTab.cpp

HEADERS += \
    FenPrincipaleNote.h \
    NoyauNote.h \
    TabNote.h \
    ZoneCentraleNote.h \
    ElevesTab.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/entities/release/ -lentities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/entities/debug/ -lentities
else:unix: LIBS += -L$$PWD/../../../../Compilation/libmps/entities/ -lentities

INCLUDEPATH += $$PWD/../../../libmps/entities
DEPENDPATH += $$PWD/../../../libmps/entities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/entities/release/libentities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/entities/debug/libentities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/entities/release/entities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/entities/debug/entities.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/entities/libentities.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/manager/release/ -lmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/manager/debug/ -lmanager
else:unix: LIBS += -L$$PWD/../../../../Compilation/libmps/manager/ -lmanager

INCLUDEPATH += $$PWD/../../../libmps/manager
DEPENDPATH += $$PWD/../../../libmps/manager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/manager/release/libmanager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/manager/debug/libmanager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/manager/release/manager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/manager/debug/manager.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/manager/libmanager.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/fen/release/ -lfen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/fen/debug/ -lfen
else:unix: LIBS += -L$$PWD/../../../../Compilation/libmps/fen/ -lfen

INCLUDEPATH += $$PWD/../../../libmps/fen
DEPENDPATH += $$PWD/../../../libmps/fen

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/fen/release/libfen.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/fen/debug/libfen.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/fen/release/fen.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/fen/debug/fen.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/fen/libfen.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/widget/release/ -lwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Compilation/libmps/widget/debug/ -lwidget
else:unix: LIBS += -L$$PWD/../../../../Compilation/libmps/widget/ -lwidget

INCLUDEPATH += $$PWD/../../../libmps/widget
DEPENDPATH += $$PWD/../../../libmps/widget

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/widget/release/libwidget.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/widget/debug/libwidget.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/widget/release/widget.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/widget/debug/widget.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../Compilation/libmps/widget/libwidget.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../managernote/release/ -lmanagernote
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../managernote/debug/ -lmanagernote
else:unix: LIBS += -L$$OUT_PWD/../managernote/ -lmanagernote

INCLUDEPATH += $$PWD/../managernote
DEPENDPATH += $$PWD/../managernote

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../managernote/release/libmanagernote.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../managernote/debug/libmanagernote.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../managernote/release/managernote.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../managernote/debug/managernote.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../managernote/libmanagernote.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../dialognote/release/ -ldialognote
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../dialognote/debug/ -ldialognote
else:unix: LIBS += -L$$OUT_PWD/../dialognote/ -ldialognote

INCLUDEPATH += $$PWD/../dialognote
DEPENDPATH += $$PWD/../dialognote

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dialognote/release/libdialognote.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dialognote/debug/libdialognote.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dialognote/release/dialognote.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../dialognote/debug/dialognote.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../dialognote/libdialognote.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../widgetnote/release/ -lwidgetnote
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../widgetnote/debug/ -lwidgetnote
else:unix: LIBS += -L$$OUT_PWD/../widgetnote/ -lwidgetnote

INCLUDEPATH += $$PWD/../widgetnote
DEPENDPATH += $$PWD/../widgetnote

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widgetnote/release/libwidgetnote.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widgetnote/debug/libwidgetnote.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widgetnote/release/widgetnote.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../widgetnote/debug/widgetnote.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../widgetnote/libwidgetnote.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../modeldelegatenote/release/ -lmodeldelegatenote
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../modeldelegatenote/debug/ -lmodeldelegatenote
else:unix: LIBS += -L$$OUT_PWD/../modeldelegatenote/ -lmodeldelegatenote

INCLUDEPATH += $$PWD/../modeldelegatenote
DEPENDPATH += $$PWD/../modeldelegatenote

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../modeldelegatenote/release/libmodeldelegatenote.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../modeldelegatenote/debug/libmodeldelegatenote.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../modeldelegatenote/release/modeldelegatenote.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../modeldelegatenote/debug/modeldelegatenote.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../modeldelegatenote/libmodeldelegatenote.a
