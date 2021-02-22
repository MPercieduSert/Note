#-------------------------------------------------
#
# Project created by QtCreator 2020-01-16T12:03:08
#
#-------------------------------------------------

QT       += widgets sql

TARGET = dialognote
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
    NewModifForm.cpp \
    ElevesDialog.cpp

HEADERS += \
    NewModifForm.h \
    ElevesDialog.h
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


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/widget/release/ -lwidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../compilation/lib2d/widget/debug/ -lwidget
else:unix: LIBS += -L$$PWD/../../../compilation/lib2d/widget/ -lwidget

INCLUDEPATH += $$PWD/../../../lib2d/widget
DEPENDPATH += $$PWD/../../../lib2d/widget

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/widget/release/libwidget.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/widget/debug/libwidget.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/widget/release/widget.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/widget/debug/widget.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../compilation/lib2d/widget/libwidget.a

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
