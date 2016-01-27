#pragma once

namespace bacchus {

class Win {
public:
    Win() {}
    virtual ~Win() {}

    virtual void onInit() = 0;
    virtual void onClose() = 0;
    virtual void onIdle() = 0;
    virtual void onRender() = 0;

    virtual void onResize(int w, int h) = 0;
    virtual void onMouseDown(int button, int x, int y) = 0;
    virtual void onMouseUp(int button, int x, int y) = 0;
    virtual void onMouseMove(int x, int y) = 0;
    virtual void onLeftMouseDrag(int x, int y) = 0;
    virtual void onMouseWheel(int wheelnum, int direction, int x, int y) = 0;
    virtual void onKeyDown(int key) = 0;
    virtual void onKeyUp(int key) = 0;

    virtual void repaint() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void close() = 0;
    virtual void fullScreen(bool full) = 0;
};

} // namespace bacchus
