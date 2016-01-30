QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = sample

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS += -lGL -lpng
#DEFINES += BCC_DEBUG_GLERROR
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive

BCC_DIR = ../../src

SOURCES += \
    src/main.cpp \
    $$BCC_DIR/filters/*.cpp \
    $$BCC_DIR/glwrap/*.cpp \
    $$BCC_DIR/image/*.cpp \
    $$BCC_DIR/math/*.cpp \
    $$BCC_DIR/utils/*.cpp \


HEADERS += \
    $$BCC_DIR/filters/*.h \
    $$BCC_DIR/glwrap/*.h \
    $$BCC_DIR/image/*.h \
    $$BCC_DIR/math/*.h \
    $$BCC_DIR/utils/*.h \


INCLUDEPATH += \
    $$BCC_DIR \
    ../../3rdparty


