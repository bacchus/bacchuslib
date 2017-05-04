QT += core gui opengl widgets
CONFIG += c++11

TARGET = sample

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS += -lglut -lGL -lGLU -lGLEW -lpng
DEFINES += LINUX=1 GL_GLEXT_PROTOTYPES
#BCC_DEBUG_GLERROR
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive

BCC_DIR = ../../src

SOURCES += \
    src/main.cpp \
    $$BCC_DIR/audio/*.cpp \
    $$BCC_DIR/cvml/*.cpp \
    $$BCC_DIR/filters/*.cpp \
    $$BCC_DIR/glwrap/*.cpp \
    $$BCC_DIR/image/*.cpp \
    $$BCC_DIR/math/*.cpp \
    $$BCC_DIR/raytrace/*.cpp \
    $$BCC_DIR/thread/*.cpp \
    $$BCC_DIR/ui/*.cpp \
    $$BCC_DIR/utils/*.cpp \


HEADERS += \
    $$BCC_DIR/audio/*.h \
    $$BCC_DIR/cvml/*.h \
    $$BCC_DIR/filters/*.h \
    $$BCC_DIR/glwrap/*.h \
    $$BCC_DIR/image/*.h \
    $$BCC_DIR/math/*.h \
    $$BCC_DIR/raytrace/*.h \
    $$BCC_DIR/thread/*.h \
    $$BCC_DIR/ui/*.h \
    $$BCC_DIR/utils/*.h \


INCLUDEPATH += \
    $$BCC_DIR \
    ../../3rdparty


