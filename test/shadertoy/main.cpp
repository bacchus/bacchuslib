#include <vector>
#include <limits>
#include <map>

#include <QApplication>
#include <QGLWidget>
#include <QKeyEvent>

#include "image/png_rw.h"
#include "filters/filter_simple.h"
#include "glwrap/program.h"
#include "utils/fileio.h"
#include "utils/timer.h"

using namespace bacchus;

//Shader Inputs
//uniform vec3      iResolution;           // viewport resolution (in pixels)
//uniform float     iGlobalTime;           // shader playback time (in seconds)
//uniform float     iTimeDelta;            // render time (in seconds)
//uniform int       iFrame;                // shader playback frame
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
//uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
//uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
//uniform vec4      iDate;                 // (year, month, day, time in seconds)
//uniform float     iSampleRate;           // sound sample rate (i.e., 44100)

class FilterRay: public Filter {
public:
    FilterRay(const std::string& vs, const std::string& fs)
        : Filter(vs,fs)
    {
        m_time.x = 0.f;
        m_mouse.x = 10;
        m_mouse.y = 10;
        addParameter("iResolution", &m_size);
        addParameter("iGlobalTime", &m_time);
        addParameter("iMouse", &m_mouse);
    }

    void setMouse(const vec2f& mouse) {
        m_mouse.x = mouse.x;
        m_mouse.y = mouse.y;
        setChanged();
    }

    void setTime(float tm) {
        m_time.x = tm;
        setChanged();
    }

    void onResize(int w, int h) {
        m_size.x = w;
        m_size.y = h;
    }

private:
    uvec1f m_time;
    uvec2f m_size;
    uvec2f m_mouse;
};

class GLTest: public QGLWidget {
public:
    GLTest(QWidget* parent = 0)
        : QGLWidget(parent)
    {
        LOGD("Hello kittie!");

        /// setup qt window
        setWindowTitle("Hello kittie");
        setAttribute(Qt::WA_QuitOnClose);
        startTimer(10);
        setMouseTracking(false);
        m_timer.start();
        m_width = 640; m_height = 480;
        resize(m_width, m_height);

        /// opengl specific
        std::string vstr = file_readbuff("../shaders/shadertoy.vert");
        std::string fstr = file_readbuff("../shaders/raymarching-primitives.frag");
        m_filter = new FilterRay(vstr, fstr);
        m_filter->resize(m_width, m_height);
        m_filter->setOutput(NULL);
    }

    ~GLTest() {
        delete m_filter;
    }

    static int main(int argc, char* argv[]) {
        srand(time(NULL));
        QApplication app(argc, argv);
        GLTest widget;
        widget.show();
        return app.exec();
    }

    typedef void (GLTest::*on_key)(int);
    std::map<int, on_key> key_press = {
        {Qt::Key_Escape, &GLTest::exit_program},
    };

    void exit_program(int) { close(); }

protected:
    virtual void keyPressEvent(QKeyEvent* event) {
        int key = event->key();
        if (key_press.count(key)) {
            (this->*key_press[key])(key);
        }
    }

    virtual void mouseMoveEvent(QMouseEvent* event) {
        QPoint qp = event->pos();
        m_mousepos = vec2f(qp.x(), qp.y());
        m_filter->setMouse(m_mousepos);
    }

    virtual void timerEvent(QTimerEvent* /*event*/) {
        m_filter->setTime(m_timer.sec());
        updateGL();
    }

    virtual void updateGL() {
        QGLWidget::updateGL();
        paintGL();
    }

    virtual void initializeGL() {
        // gl settings
        CHECK_GL_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        CHECK_GL_ERROR(glEnable(GL_BLEND));
        CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
        // clearing
        CHECK_GL_ERROR(glClearColor(0.7, 0.5, 0.2, 1.0));
        CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT));
        // init glwraps
        m_filter->init();
    }

    virtual void resizeGL(int w, int h) {
        m_filter->resize(w, h);
    }

    virtual void paintGL() {
        m_fps.begin();
        m_filter->render();
        m_fps.end();
    }

private:
    int m_width, m_height;
    FilterRay* m_filter;
    vec2f m_mousepos;
    Timer m_timer;
    FpsTimer m_fps;
};

int main(int argc, char* argv[]) {
    return GLTest::main(argc, argv);
}