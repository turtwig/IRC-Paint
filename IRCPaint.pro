#-------------------------------------------------
#
# Project created by QtCreator 2010-07-12T19:32:25
#
#-------------------------------------------------

QT       += core gui

TARGET = IRCPaint
TEMPLATE = app

ICON = IRCPaint.icns
win32 {
    RC_FILE = IRCPaint.rc
}

SOURCES += main.cpp\
        IRCPaintMainWindow.cpp \
    MainWidget.cpp \
    Brush.cpp \
    Brush_Pen.cpp \
    Brush_Line.cpp \
    Brush_Rect.cpp

HEADERS  += IRCPaintMainWindow.h \
    MainWidget.h \
    Brush.h \
    Brush_Pen.h \
    Brush_Line.h \
    Brush_Rect.h

OTHER_FILES += \
    IRCPaint.rc

RESOURCES += \
    IRCPaint.qrc
