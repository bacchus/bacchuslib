OBJECTS_DIR = obj
DESTDIR = bin

QT += opengl

#QT -= core qui console windows opengl
#QMAKE_CXXFLAGS += -ansi -pedantic -Wconversion -Wshadow -Wcast-qual -Wwrite-strings
# defaults: -Wall -W


LIBS += -lGL

INCLUDEPATH += \


SOURCES += \
    src/main.cpp


HEADERS += \


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
