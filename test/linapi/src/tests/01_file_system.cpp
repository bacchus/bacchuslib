#include "gtest/gtest.h"

#include "apis/filesystem/cdriper.h"
#include "apis/filesystem/print_env.h"
#include "apis/filesystem/make_big_file.h"
#include "apis/filesystem/print_dir.h"
#include "apis/filesystem/file_locks.h"

//TEST(file_system, CdEject) {
//    using namespace cdriper;
//    cd_eject();
//}

TEST(file_system, PrintEnv) {
//    print_env();
}

TEST(file_system, MakeHole) {
//    make_big_file("big_file", 0x1000000);
//    std::cout << "file size: " << file_size("big_file") << std::endl;
//    std::cout << "created  : " << 0x1000000 << std::endl;
}

TEST(file_system, PrintDir1) {
//    scan_dir(".");
}

TEST(file_system, PrintDir2) {
//    open_dir(".");
}

TEST(file_system, TestLocks) {
//    test_locks();
}
