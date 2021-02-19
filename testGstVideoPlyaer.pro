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

win32: LIBS += -L$$PWD/lib/

win32: LIBS += -lgstreamer-1.0
win32: LIBS += -L$$PWD/lib/ -lglib-2.0
win32: LIBS += -L$$PWD/lib/ -lgio-2.0
win32: LIBS += -L$$PWD/lib/ -lgstapp-1.0
win32: LIBS += -L$$PWD/lib/ -lgstbase-1.0


HEADERS += \
    gstvideoplayer.h

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/gstreamer-1.0
INCLUDEPATH += $$PWD/include/glib-2.0
INCLUDEPATH += $$PWD/include/glib-2.0/gio
INCLUDEPATH += $$PWD/include/glib-2.0/glib
INCLUDEPATH += $$PWD/include/glib-2.0/gobject

DEPENDPATH +=$$PWD/include
DEPENDPATH +=$$PWD/include/gstreamer-1.0
DEPENDPATH +=$$PWD/include/glib-2.0
DEPENDPATH += $$PWD/include/glib-2.0/gio
DEPENDPATH += $$PWD/include/glib-2.0/glib
DEPENDPATH += $$PWD/include/glib-2.0/gobject

