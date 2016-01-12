CONFIG -= qt
CONFIG += c++11
TARGET = unit_tests

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
CCFLAG +=  -Wno-pointer-to-int-cast
#-std=c++0x -fpermissive -Wno-permissive
LIBS += -lpng -lpthread

SOURCES += \
    main.cpp \
    ../../3rdparty/gtest/gtest-all.cpp \
    ../../src/cvml/*.cpp \
    ../../src/audio/*.cpp \
    ../../src/math/*.cpp \
    ../../src/image/*.cpp \
    ../../src/utils/*.cpp \
    tests/sample_test.cpp \
    tests/01_vec_tests.cpp \
    tests/02_matalg_test.cpp \
    tests/03_linear_prog_test.cpp \
    tests/04_graph_test.cpp \
    tests/05_sort_test.cpp \
    tests/06_fft_test.cpp \
    tests/07_substr_test.cpp \
    tests/09_png_converter.cpp \
    tests/10_gauss_invert.cpp \
    tests/11_hough_test.cpp \
    tests/12_geometric_test.cpp \
    tests/13_cvml_linear_regression.cpp \
    tests/14_cvml_logistic_regression.cpp \
    tests/15_cvml_neural_network.cpp \
    tests/16_audio-p2.cpp \
    tests/17_audio-p3.cpp \
    tests/18_FFT_revtests.cpp \
    tests/19_fft_double.cpp \


HEADERS += \
    setting.h \
    ../../src/audio/*.h \
    ../../src/cvml/*.h \
    ../../src/math/*.h \
    ../../src/image/*.h \
    ../../src/utils/*.h \


INCLUDEPATH += \
    ../../ \
    ../../src \
    ../../3rdparty
