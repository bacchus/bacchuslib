OBJECTS_DIR = obj
DESTDIR = bin

QT -= core qui console windows opengl

#QMAKE_CXXFLAGS += -ansi -pedantic -Wconversion -Wshadow -Wcast-qual -Wwrite-strings
# defaults: -Wall -W


LIBS += \
    -lpthread


INCLUDEPATH += \
    ../../src


SOURCES += \
    src/hello.cpp \
    src/test.cpp \
    src/fractal.cpp


HEADERS += \
    ../../src/thread/thread.h


FORMS += \



#TEMPLATE = app
#TARGET = [app name]
#VERSION = 0.0.1
#CONFIG -= debug_and_release app_bundle lib_bundle
#CONFIG += [debug|release]
#RESOURCES += icons.qrc
#MOC_DIR = qt
#UI_DIR = qt
#DEFINES += 

