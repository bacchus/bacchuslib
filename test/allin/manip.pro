QT += core gui opengl widgets
CONFIG += c++11

TARGET = manip

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS += -lglut -lGL -lGLU -lGLEW -lpng
DEFINES += LINUX=1 GL_GLEXT_PROTOTYPES
#BCC_DEBUG_GLERROR
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive

SOURCES += \
    main.cpp \
    ../../src/audio/*.cpp \
    ../../src/cvml/*.cpp \
    ../../src/filters/*.cpp \
    ../../src/glwrap/*.cpp \
    ../../src/image/*.cpp \
    ../../src/math/*.cpp \
    ../../src/raytrace/*.cpp \
    ../../src/utils/*.cpp \


HEADERS += \
    ../../src/audio/*.h \
    ../../src/cvml/*.h \
    ../../src/filters/*.h \
    ../../src/glwrap/*.h \
    ../../src/image/*.h \
    ../../src/math/*.h \
    ../../src/raytrace/*.h \
    ../../src/utils/*.h \
    ../../src/glwrap/glinclude.h

INCLUDEPATH += \
    ../../ \
    ../../src \
    ../../3rdparty
