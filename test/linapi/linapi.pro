OBJECTS_DIR = obj
DESTDIR = bin

CONFIG -= qt
CONFIG += c++11

LIBS += -lpthread
DEFINES += _REENTERANT

INCLUDEPATH += \
    . \
    src \
    ../../3rdparty \

SOURCES += \
    src/main.cpp \
    ../../3rdparty/gtest/gtest-all.cpp \
    src/tests/sample_test.cpp \
    src/tests/01_file_system.cpp \
    src/tests/02_ipc.cpp \
    src/tests/03_svid.cpp \
    src/tests/04_sockets.cpp \
    src/tests/05_signals.cpp \
    src/tests/06_processes.cpp \
    src/tests/07_threads.cpp




HEADERS += \
    src/apis/filesystem/print_env.h \
    src/apis/filesystem/make_big_file.h \
    src/apis/filesystem/print_dir.h \
    src/apis/filesystem/file_locks.h \
    src/apis/ipc/pipes.h \
    src/apis/svid/svid_msg.h \
    src/apis/svid/svid_shmem.h \
    src/apis/svid/svid_sem.h \
    src/apis/socket/socket_pair.h \
    src/apis/socket/file_sockets.h \
    src/apis/socket/net_sockets.h \
    src/apis/signals/sigdemo.h \
    src/apis/process/execs.h \
    src/apis/process/nozombies.h \
    src/apis/process/execsystem.h \
    src/apis/threads/threads.h \
    src/apis/filesystem/cdriper.h



#FORMS +=
#TEMPLATE = app
#TARGET = [app name]
#VERSION = 0.0.1
#CONFIG -= debug_and_release app_bundle lib_bundle
#CONFIG += [debug|release]
#RESOURCES += icons.qrc
#DEFINES +=
#MOC_DIR = tmp
#UI_DIR = tmp
#RCC_DIR = tmp
#UI_HEADERS_DIR = tmp
#UI_SOURCES_DIR = tmp
