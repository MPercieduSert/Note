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
    noyau_note.cpp \
    tab_note.cpp \
    eleve_tab.cpp \
    exercice_tab.cpp \
    groupe_tab.cpp \
    zone_centrale_note.cpp \
    fen_principale_note.cpp

HEADERS += \
    noyau_note.h \
    tab_note.h \
    eleve_tab.h \
    exercice_tab.h \
    groupe_tab.h \
    zone_centrale_note.h \
    fen_principale_note.h \
    enum_fen_note.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/entities/release/ -lentities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/entities/debug/ -lentities
else:unix: LIBS += -L$$PWD/../../../compilation/libmps/entities/ -lentities

INCLUDEPATH += $$PWD/../../../libmps/entities
DEPENDPATH += $$PWD/../../../libmps/entities

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/entities/release/libentities.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/entities/debug/libentities.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/entities/release/entities.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/entities/debug/entities.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/entities/libentities.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/manager/release/ -lmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/manager/debug/ -lmanager
else:unix: LIBS += -L$$PWD/../../../compilation/libmps/manager/ -lmanager

INCLUDEPATH += $$PWD/../../../libmps/manager
DEPENDPATH += $$PWD/../../../libmps/manager

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/manager/release/libmanager.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/manager/debug/libmanager.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/manager/release/manager.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/manager/debug/manager.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/manager/libmanager.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/widget/release/ -lwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/widget/debug/ -lwidget
else:unix: LIBS += -L$$PWD/../../../compilation/libmps/widget/ -lwidget

INCLUDEPATH += $$PWD/../../../libmps/widget
DEPENDPATH += $$PWD/../../../libmps/widget

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/widget/release/libwidget.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/widget/debug/libwidget.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/widget/release/widget.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/widget/debug/widget.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/widget/libwidget.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/fen/release/ -lfen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/fen/debug/ -lfen
else:unix: LIBS += -L$$PWD/../../../compilation/libmps/fen/ -lfen

INCLUDEPATH += $$PWD/../../../libmps/fen
DEPENDPATH += $$PWD/../../../libmps/fen

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/fen/release/libfen.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/fen/debug/libfen.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/fen/release/fen.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/fen/debug/fen.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/fen/libfen.a


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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/libmps/model/debug/ -lmodel
else:unix: LIBS += -L$$PWD/../../../compilation/libmps/model/ -lmodel

INCLUDEPATH += $$PWD/../../../libmps/model
DEPENDPATH += $$PWD/../../../libmps/model

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/model/release/libmodel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/model/debug/libmodel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/model/release/model.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/model/debug/model.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/libmps/model/libmodel.a
