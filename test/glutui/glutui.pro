OBJECTS_DIR = obj
DESTDIR = bin

#QT -= core qui console windows opengl

QT += opengl

#QMAKE_CXXFLAGS += -ansi -pedantic -Wconversion -Wshadow -Wcast-qual -Wwrite-strings
# defaults: -Wall -W


LIBS += -lGLEW -lGL -lglut -lGLU


INCLUDEPATH += \
    ../../src


SOURCES += \
    src/testwinapp.cpp \
    ../../src/ui/glutwin.cpp



HEADERS += \
    ../../src/ui/win.h \
    ../../src/ui/glutwin.h \
    ../../src/ui/app.h \


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

OTHER_FILES += \
    shaders/lighting.frag \
    shaders/lighting.vert
