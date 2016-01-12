QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = bccrt

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS += -lGL -lpng
#DEFINES += BCC_DEBUG_GLERROR
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive

SOURCES += \
    main.cpp \
    ../../src/filters/*.cpp \
    ../../src/glwrap/*.cpp \
    ../../src/image/*.cpp \
    ../../src/math/*.cpp \
    ../../src/utils/*.cpp

HEADERS += \
    *.h \
    ../../src/filters/*.h \
    ../../src/glwrap/*.h \
    ../../src/image/*.h \
    ../../src/math/*.h \
    ../../src/utils/*.h

INCLUDEPATH += \
    ../../src \
    ../../

