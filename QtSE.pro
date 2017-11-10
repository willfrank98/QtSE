#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T14:03:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSE
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
    model.cpp \
    canvas.cpp \
    frame.cpp

HEADERS += \
        mainwindow.h \
    model.h \
    canvas.h \
    frame.h \
    tool.h \
    gif.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc


# Link ImageMagick stuff
#unix: {
#    LIBS += -L/usr/lib -lMagick -lMagick++
#    INCLUDEPATH += /usr/include
#}
#else:unix:!macx {
#    # If linking doesn't work on your Mac, just replace these with what you did last time.
#    LIBS += -L/usr/lib -lMagick -lMagick++
#    INCLUDEPATH += /usr/include
#}
#else:win32:contains(QT_ARCH, x86_64) {
#    LIBS += -L$$PWD/ImageMagick/win64/lib -lCORE_RL_MagickCore_ -lCORE_RL_Magick++_
#    INCLUDEPATH += $$PWD/ImageMagick/win64/include
#    DEPENDPATH += $$PWD/ImageMagick/win64/lib
#    PRE_TARGETDEPS += $$PWD/ImageMagick/win64/lib/CORE_RL_MagickCore_.lib $$PWD/ImageMagick/win64/lib/CORE_RL_Magick++_.lib
#    QMAKE_POST_LINK = xcopy /d \"$$PWD/ImageMagick/win64/bin\" \"$$OUT_PWD\"
#}
#else:win32:contains(QT_ARCH, i386) {
#    LIBS += -L$$PWD/ImageMagick/win32/lib -lCORE_RL_MagickCore_ -lCORE_RL_Magick++_
#    INCLUDEPATH += $$PWD/ImageMagick/win32/include
#    DEPENDPATH += $$PWD/ImageMagick/win32/lib
#    PRE_TARGETDEPS += $$PWD/ImageMagick/win32/lib/CORE_RL_MagickCore_.lib $$PWD/ImageMagick/win32/lib/CORE_RL_Magick++_.lib
#    QMAKE_POST_LINK = xcopy /d \"$$PWD/ImageMagick/win32/bin\" \"$$OUT_PWD\"
#}
