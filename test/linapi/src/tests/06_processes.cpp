#include "gtest/gtest.h"
#include "apis/process/execs.h"
#include "apis/process/nozombies.h"
#include "apis/process/execsystem.h"

TEST(Processes, Exec) {
    //const char* argv[] = {"ls", "-al", NULL};
    //exec(const_cast<char**>(argv));
}

TEST(Processes, Nozombie) {
    //nozombie();
}

TEST(Processes, System) {
    //execsystem("ls -al");
}
