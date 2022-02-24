QT       += core gui webenginewidgets network multimedia axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
OBJECTS_DIR  = tmp
MOC_DIR      = tmp

INCLUDEPATH += $$PWD/mupdf/include/
QMAKE_CFLAGS_DEBUG += -MDd
QMAKE_CXXFLAGS_DEBUG += -MDd
QMAKE_CFLAGS_RELEASE += -MD
QMAKE_CXXFLAGS_RELEASE += -MD
CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET = X-pdfshow
RC_ICONS = exelogo.ico
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myslider.cpp \
    volume_slider.cpp

HEADERS += \
    mainwindow.h \
    myslider.h \
    volume_slider.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Fonts.qrc

FORMS +=
win32-msvc*: {
    QMAKE_CFLAGS *= /utf-8
    QMAKE_CXXFLAGS *= /utf-8
}





#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/release/ -llibmupdf
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/debug/ -llibmupdf

#INCLUDEPATH += $$PWD/mupdf/platform/win32/x64
#DEPENDPATH += $$PWD/mupdf/platform/win32/x64

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/liblibmupdf.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/liblibmupdf.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/libmupdf.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/libmupdf.lib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/release/ -llibthirdparty
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/debug/ -llibthirdparty

#INCLUDEPATH += $$PWD/mupdf/platform/win32/x64
#DEPENDPATH += $$PWD/mupdf/platform/win32/x64

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/liblibthirdparty.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/liblibthirdparty.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/libthirdparty.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/libthirdparty.lib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/release/ -llibresources
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/debug/ -llibresources

#INCLUDEPATH += $$PWD/mupdf/platform/win32/x64
#DEPENDPATH += $$PWD/mupdf/platform/win32/x64

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/liblibresources.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/liblibresources.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/libresources.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/libresources.lib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/release/ -llibresources
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/mupdf/platform/win32/x64/debug/ -llibresources

#INCLUDEPATH += $$PWD/mupdf/platform/win32/x64
#DEPENDPATH += $$PWD/mupdf/platform/win32/x64

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/liblibresources.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/liblibresources.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/release/libresources.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mupdf/platform/win32/x64/debug/libresources.lib
