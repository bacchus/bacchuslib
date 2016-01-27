#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

namespace bacchus {

class App {
public:
    App(char* app_name) {
        mArgc = 1;
        mArgv = new char*[1];
        mArgv[0] = app_name;
        glutInit(&mArgc, mArgv);
    }

    App(int argc, char** argv)
        : mArgc(argc), mArgv(argv)
    {
        glutInit(&mArgc, mArgv);
    }

    virtual ~App() {}

    virtual void onInit() {}
    virtual void onExit() {}

    virtual void mainLoop() {
        glutMainLoop();
    }

    void run() {
        onInit();
        mainLoop();
        onExit();
    }

    int argc() const {
        return mArgc;
    }

    char** argv() const {
        return mArgv;
    }

private:
    int mArgc;
    char** mArgv;
};

} // namespace bacchus
