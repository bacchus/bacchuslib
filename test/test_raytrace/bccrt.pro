QT += core gui opengl widgets
CONFIG += c++11

TARGET = bccrt

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS += -lglut -lGL -lGLU -lGLEW -lpng
DEFINES += LINUX=1 GL_GLEXT_PROTOTYPES
#BCC_DEBUG_GLERROR
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive

SOURCES += \
    ../../src/filters/*.cpp \
    ../../src/glwrap/*.cpp \
    ../../src/image/*.cpp \
    ../../src/math/*.cpp \
    ../../src/utils/*.cpp \
    src/main.cpp

HEADERS += \
    ../../src/filters/*.h \
    ../../src/glwrap/*.h \
    ../../src/image/*.h \
    ../../src/math/*.h \
    ../../src/utils/*.h \
    src/bcctrace.h

INCLUDEPATH += \
    ../../3rdparty \
    ../../src \
    ../../

