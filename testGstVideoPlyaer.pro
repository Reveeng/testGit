QT += quick multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gstvideoplayer.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += $$PWD/lib/libgio-2.0-0.dll
win32: LIBS += $$PWD/lib/libgstapp-1.0-0.dll
win32: LIBS += $$PWD/lib/libgstbase-1.0-0.dll
win32: LIBS += $$PWD/lib/libgstreamer-1.0-0.dll
win32: LIBS += $$PWD/lib/libgobject-2.0-0.dll
win32: LIBS += $$PWD/lib/libglib-2.0-0.dll


HEADERS += \
    gstvideoplayer.h

INCLUDEPATH += $$PWD/gstreamer-1.0
INCLUDEPATH += $$PWD/glib-2.0
INCLUDEPATH += $$PWD/glib-2.0/gobject
INCLUDEPATH += $$PWD/glib-2.0/glib
INCLUDEPATH += $$PWD/glib-2.0/gio

DEPENDPATH +=$$PWD/gstreamer-1.0
DEPENDPATH +=$$PWD/glib-2.0
DEPENDPATH +=$$PWD/glib-2.0/gobject
DEPENDPATH +=$$PWD/glib-2.0/glib
DEPENDPATH +=$$PWD/glib-2.0/gio

GST_DEBUG = 3
