CONFIG -= qt
CONFIG += c++11
CONFIG -= debug_and_release

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
    tests/02_rbtree_test.cpp \
    tests/03_merge_heap.cpp \
    tests/04_graph_test.cpp \
    ../../src/math/graph.cpp \
    ../../src/math/fft.cpp \
    ../../src/math/linear_prog.cpp \
    ../../src/math/matalg.cpp \
    ../../src/math/substring.cpp \
    ../../src/math/fib_heap.cpp \
    tests/05_fib_heap.cpp


HEADERS += \
    setting.h \
    ../../src/math/*.h \
    ../../src/utils/*.h \


INCLUDEPATH += \
    ../../ \
    ../../src \
    ../../3rdparty
