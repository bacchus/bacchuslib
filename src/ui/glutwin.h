#pragma once

#include <map>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "win.h"

#include "math/vec2.h"

namespace bacchus {

class glutWin: public Win {
public:
    glutWin(const vec2i& pos = vec2i(100, 100),
            const vec2i& size = vec2i(640, 480),
            const std::string& capt = "untitled") {
        init(pos, size, capt);
    }

    void init(const vec2i& pos,
              const vec2i& size,
              const std::string& capt) {
        glutInitWindowSize(size.x, size.y);
        glutInitWindowPosition(pos.x, pos.y);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        mHandle = glutCreateWindow(capt.c_str());

        glewInit();
        if (glewIsSupported("GL_VERSION_2_0"))
            std::cout << "Ready for OpenGL 2.0" << std::endl;
        else {
            std::cout << "OpenGL 2.0 not supported" << std::endl;
            exit(1);
        }

        mWinHandles[mHandle] = this;
        setupEventHandlers();
        mInited = false;
    }

    virtual ~glutWin() {
        std::map<int, glutWin*>::iterator i = mWinHandles.find(mHandle);
        if (i!=mWinHandles.end()) {
            mWinHandles.erase(mHandle);
        }
    }

    virtual void onInit() {}
    virtual void onClose() {}
    virtual void onIdle() {}
    virtual void onRender() {}
    virtual void onResize(int /*w*/, int /*h*/) {}
    virtual void onMouseDown(int /*button*/, int /*x*/, int /*y*/) {}
    virtual void onMouseUp(int /*button*/, int /*x*/, int /*y*/) {}
    virtual void onMouseMove(int /*x*/, int /*y*/) {}
    virtual void onLeftMouseDrag(int /*x*/, int /*y*/) {}
    virtual void onMouseWheel(int /*wheelnum*/, int /*direction*/, int /*x*/, int /*y*/) {}
    virtual void onKeyDown(int /*key*/) {}
    virtual void onKeyUp(int /*key*/) {}

    virtual void repaint() {
        glutPostWindowRedisplay(mHandle);
    }

    virtual void show() {
        glutShowWindow();
    }

    virtual void hide() {
        glutHideWindow();
    }

    virtual void close() {
        glutDestroyWindow(mHandle);
    }

    virtual void fullScreen(bool full) {
        if (full) {
            glutFullScreen();
        } else {
            int w = glutGet(GLUT_WINDOW_WIDTH);
            int h = glutGet(GLUT_WINDOW_HEIGHT);
            int x = glutGet(GLUT_WINDOW_X);
            int y = glutGet(GLUT_WINDOW_Y);
            glutReshapeWindow(w, h);
            glutPositionWindow(x, y);
        }
    }

    vec2i size() {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        return vec2i(w, h);
    }

private:
    void setupEventHandlers() {
        glutIdleFunc(idleFunc);
        glutCloseFunc(closeFunc);
        glutDisplayFunc(renderFunc);
        glutReshapeFunc(resizeFunc);
        glutMouseFunc(mouseFunc);
        glutMotionFunc(mouseMoveFunc);
        glutMouseWheelFunc(mouseWheelFunc);
        glutKeyboardFunc(keyDownFunc);
        glutKeyboardUpFunc(keyUpFunc);
        //glutSpecialFunc(keyDownFunc);
        //glutSpecialUpFunc(keyUpFunc);
    }

    static glutWin* current() {
        int cur = glutGetWindow();
        std::map<int, glutWin*>::iterator i = mWinHandles.find(cur);
        if (i!=mWinHandles.end()) {
            return i->second;
        }
        return NULL;
    }

    static void idleFunc() {
        glutWin* cur = current();
        if (cur) {
            cur->onIdle();
        }
    }

    static void closeFunc() {
        glutWin* cur = current();
        if (cur) {
            cur->onClose();
        }
    }

    static void resizeFunc(int w, int h) {
        glutWin* cur = current();
        if (cur) {
            cur->onResize(w, h);
        }
    }

    static void renderFunc() {
        glutWin* cur = current();
        if (cur) {
            if (!cur->mInited) {
                cur->onInit();
                cur->mInited = true;
            }
            cur->onRender();
        }
    }

    static void mouseFunc(int button, int updown, int x, int y) {
        glutWin* cur = current();
        if (cur) {
            if (updown) {
                if (button==0)
                    mIsLeftMouseDown = false;
                cur->onMouseUp(button, x, y);
            } else {
                if (button==0)
                    mIsLeftMouseDown = true;
                cur->onMouseDown(button, x, y);
            }

            if (button==3) {
                cur->onMouseWheel(button, 1, x, y);
            } else if (button==4) {
                cur->onMouseWheel(button, -1, x, y);
            }
        }
    }

    static void mouseMoveFunc(int x, int y) {
        glutWin* cur = current();
        if (cur) {
            if (mIsLeftMouseDown)
                cur->onLeftMouseDrag(x, y);
            cur->onMouseMove(x, y);
        }
    }

    static void mouseWheelFunc(int wheelnum, int direction, int x, int y) {
        glutWin* cur = current();
        if (cur) {
            cur->onMouseWheel(wheelnum, direction, x, y);
        }
    }

    static void keyUpFunc(unsigned char key, int /*x*/, int /*y*/) {
        glutWin* cur = current();
        if (cur) {
            cur->onKeyUp(key);
        }
    }

    static void keyDownFunc(unsigned char key, int /*x*/, int /*y*/) {
        glutWin* cur = current();
        if (cur) {
            cur->onKeyDown(key);
        }
    }

private:
    int mHandle;
    bool mInited;
    static std::map<int, glutWin*> mWinHandles;
    static bool mIsLeftMouseDown;
};

} // namespace bacchus
