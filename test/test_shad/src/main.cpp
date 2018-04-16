#include <vector>
#include <limits>
#include <map>

#include <QApplication>
#include <QGLWidget>
#include <QKeyEvent>

#include "image/png_rw.h"
#include "filters/filter_simple.h"
#include "glwrap/program.h"

const float g_start_param = 1.0f;
const float g_delta_param = 0.1f;
const float g_min_param = 0.0f;
const float g_max_param = 1.0f;

using namespace bacchus;

static std::string vertSrc =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static std::string fragSrc =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "uniform int count;\n"
        "void main() {\n"
        "   for (int i = 0; i < count || count < 0; ++i)\n"
        "       gl_FragColor = vec4(count,count,count,count);\n"
        "}\n";

class FilterInf: public Filter {
public:
    FilterInf()
        : Filter(vertSrc, fragSrc) {
        addParameter("count", &m_count);
        m_count.x = -1;
    }

    void setData(int count) {
        m_count.x = count;
        setChanged();
    }

private:
    uvec1i m_count;
};

class GLTest: public QGLWidget {
public:
    GLTest(QWidget* parent = 0)
        : QGLWidget(parent)
    {
        LOGD("Hello kittie!");

        bacchus::read_png_mirrored("../img/test.png", m_img);

        /// setup qt window
        setWindowTitle("Hello kittie");
        setAttribute(Qt::WA_QuitOnClose);
        startTimer(10);
        m_width = m_img.width;
        m_height = m_img.height;
        resize(m_width, m_height);

        /// opengl specific
        m_tex = new bacchus::Texture(m_img.width, m_img.height, m_img.data());
        m_filter = new FilterInf();
        m_filter->setRatio(m_width/(float)m_height);
        m_filter->resize(m_width, m_height);
        m_filter->setInput(m_tex);
        m_filter->setOutput(NULL);
        m_data = g_start_param;
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
        {Qt::Key_Left, &GLTest::less},
        {Qt::Key_Right, &GLTest::more}
    };

    void exit_program(int) { close(); }

    void less(int) {
        m_data = std::max(m_data - g_delta_param, g_min_param);
        //m_filter->setChanged();
    }

    void more(int) {
        m_data = std::min(m_data + g_delta_param, g_max_param);
        //m_filter->setChanged();
    }

protected:
    virtual void keyPressEvent(QKeyEvent* event) {
        int key = event->key();
        if (key_press.count(key)) {
            (this->*key_press[key])(key);
        }
    }

    virtual void timerEvent(QTimerEvent* /*event*/) {
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
        m_tex->createGl();
        m_tex->pixels();
        m_filter->init();
        m_filter->setData(-1);
    }

    virtual void resizeGL(int w, int h) {
        m_filter->resize(w, h);
    }

    virtual void paintGL() {
        m_filter->render();
        draw_text("Hello kittie!");
    }

    void draw_text(const std::string& s) {
        QString text = QString(s.c_str());//.arg(mValue);
        int length = QFontMetrics(font()).width(text);
        glColor3f(1, 1, 1);
        renderText(width() - length - 1, font().pointSize() + 1, text);
    }

private:
    bacchus::Image m_img;
    int m_width, m_height;
    bacchus::Texture* m_tex;
    FilterInf* m_filter;
    float m_data;
};

int main(int argc, char* argv[]) {
    return GLTest::main(argc, argv);
}
