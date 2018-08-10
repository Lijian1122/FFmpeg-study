#-------------------------------------------------
#
# Project created by QtCreator 2018-08-03T14:27:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    videoplayer.cpp

HEADERS  += widget.h \
    videoplayer.h

FORMS    += widget.ui

INCLUDEPATH += $$PWD/ffmpeg/include \
               $$PWD/SDL2/include \


LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib\
        $$PWD/ffmpeg/lib/avfilter.lib\
        $$PWD/ffmpeg/lib/avformat.lib\
        $$PWD/ffmpeg/lib/avutil.lib\
        $$PWD/ffmpeg/lib/postproc.lib\
        $$PWD/ffmpeg/lib/swresample.lib\
        $$PWD/ffmpeg/lib/swscale.lib \
        $$PWD/SDL2/lib/x86/SDL2.lib \

RESOURCES += \
    videoplayer.qrc
