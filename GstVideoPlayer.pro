QT += quick multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        gstvideoplayer.cpp \
        main.cpp \
        onvif.cpp \
        snapshot.cpp \
        systemcom.cpp

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

LIBS += -llibgio-2.0-0 -llibgstapp-1.0-0 -llibgstbase-1.0-0 -llibgstreamer-1.0-0 -llibgobject-2.0-0 -llibglib-2.0-0


HEADERS += \
    gstvideoplayer.h \
    onvif.h \
    snapshot.h \
    systemcom.h


INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/gstreamer-1.0
INCLUDEPATH += $$PWD/include/glib-2.0
INCLUDEPATH += $$PWD/include/glib-2.0/gio
INCLUDEPATH += $$PWD/include/glib-2.0/glib
INCLUDEPATH += $$PWD/include/glib-2.0/gobject
INCLUDEPATH += $$PWD/include/gio-win32-2.0

DEPENDPATH +=$$PWD/include
DEPENDPATH +=$$PWD/include/gstreamer-1.0
DEPENDPATH +=$$PWD/include/glib-2.0
DEPENDPATH += $$PWD/include/glib-2.0/gio
DEPENDPATH += $$PWD/include/glib-2.0/glib
DEPENDPATH += $$PWD/include/glib-2.0/gobject
DEPENDPATH += $$PWD/include/gio-win32-2.0

DISTFILES +=


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../deconv-fft-lib/release/ -ldeconv-fft-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../deconv-fft-lib/debug/ -ldeconv-fft-lib

INCLUDEPATH += $$PWD/../deconv-fft-lib
DEPENDPATH += $$PWD/../deconv-fft-lib


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tfm-contraster-lib/release/ -ltfm-contraster-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tfm-contraster-lib/debug/ -ltfm-contraster-lib

INCLUDEPATH += $$PWD/../tfm-contraster-lib
DEPENDPATH += $$PWD/../tfm-contraster-lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tfm-temperature-lib/release/ -ltfm-temperature-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tfm-temperature-lib/debug/ -ltfm-temperature-lib

INCLUDEPATH += $$PWD/../tfm-temperature-lib
DEPENDPATH += $$PWD/../tfm-temperature-lib
