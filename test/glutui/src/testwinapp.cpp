#include <iostream>

#include "ui/app.h"
#include "ui/glutwin.h"

class myWin: public bacchus::glutWin {
public:
    void onInit() {
        a = 0.0f;
        mPos[2] = -10.0f;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0);
        glEnable(GL_TEXTURE_2D);

        float As[4] = {0.1f, 0.1f, 0.1f, 1.0f };
        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, As );

        float Al[4] = {0.0f, 0.0f, 0.0f, 1.0f };
        glLightfv( GL_LIGHT0, GL_AMBIENT, Al );

        float Dl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
        glLightfv( GL_LIGHT0, GL_DIFFUSE, Dl );

        float Sl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
        glLightfv( GL_LIGHT0, GL_SPECULAR, Sl );

        float Am[4] = {0.3f, 0.3f, 0.3f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Am );

        float Dm[4] = {0.9f, 0.5f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dm );

        float Sm[4] = {0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Sm );

        float f = 60.0f;
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, f );

        //bacchus::glprogram.use() - can add shader
        // see folder "shaders" for use of old OpenGL 2.0 shaders
    }

    void onResize(int w, int h) {
        if (h == 0) h = 1;
        float ratio = (float)w/(float)h;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, w, h);
        gluPerspective(45.0f, ratio, 1.0f, 10000.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    void onRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glLoadIdentity();

        glTranslatef(mPos[0], mPos[1], mPos[2]);
        glRotatef(a, 0.2f, 1.0f, 0.3f);

        glutSolidTeapot(1);
        //glutSolidRhombicDodecahedron();

        a += 0.1f;

        glutSwapBuffers();
    }

    void onIdle() {
        repaint();
    }

    void onKeyDown(int key) {
        if (key == 27)
            close();
    }

    void onKeyUp(int key) {
        if (key == 'f')
            fullScreen(true);
        else if (key == 'w')
            fullScreen(false);
    }

    void onLeftMouseDrag(int x, int y) {
        mPos[0] = 12.0f*x/(float)size().x - 6.0f;
        mPos[1] = -8.0f*y/(float)size().y + 4.0f;
    }

    void onMouseWheel(int, int dir, int, int) {
        mPos[2] += dir*0.2f;
    }

private:
    float mPos[3];
    float a;
};

int main(int argc, char* argv[]) {
    bacchus::App app(argv[0]);
    myWin win1;
    myWin win2;
    app.run();
    return 0;
}
