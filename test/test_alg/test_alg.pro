CONFIG -= qt
CONFIG += c++11

TARGET = test_alg

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj

CCFLAG +=  -Wno-pointer-to-int-cast


SOURCES += \
    main.cpp \
    ../../3rdparty/gtest/gtest-all.cpp \
    tests/sample_test.cpp \
    tests/01_sort_test.cpp \


HEADERS += \
    setting.h \
    ../../src/math/*.h \
    ../../src/utils/*.h \

    
INCLUDEPATH += \
    ../../ \
    ../../src \
    ../../3rdparty
