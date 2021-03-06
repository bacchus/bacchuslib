#include <vector>
#include <limits>
#include <map>

#include <QApplication>
#include <QGLWidget>
#include <QKeyEvent>

#include "image/png_rw.h"
#include "image/tga_rw.h"
#include "filters/filter_simple.h"
#include "glwrap/program.h"
#include "raytrace/obj_model.h"
#include "filter_model3d.h"
#include "math/transform.h"

const float g_start_param = 1.0f;
const float g_delta_param = 0.1f;
const float g_min_param = 0.0f;
const float g_max_param = 1.0f;

using namespace bacchus;

class GLTest: public QGLWidget {
public:
    GLTest(QWidget* parent = 0)
        : QGLWidget(parent)
    {
        LOGD("Hello kittie!");

        bacchus::read_tga("../res/diffuse.tga", m_img);
        //bacchus::read_png_mirrored("../res/test.png", m_img);
        m_model = new bacchus::Model("../res/model.obj");

        /// setup qt window
        setWindowTitle("Hello kittie");
        setAttribute(Qt::WA_QuitOnClose);
        startTimer(10);
        m_width = m_img.width;
        m_height = m_img.height;
        resize(m_width, m_height);

        /// opengl specific
        m_tex = new bacchus::Texture(m_img.width, m_img.height, m_img.data());
        m_tex->format(GL_BGR);
        m_tex->mipmap(true);


        m_filter = new bacchus::FilterModel3d(*m_model);
        m_filter->resize(m_width, m_height);
        m_filter->setRatio(m_width/float(m_height));

        cam_tar = m_model->center();
        cam_pos = cam_tar + 2.0f*m_model->radius()*vec3f(0,0,1);
        updateCam();

        m_filter->setInput(m_tex);
        m_filter->setOutput(nullptr);
        m_data = g_start_param;
    }

    ~GLTest() {
        delete m_filter;
        delete m_tex;
        delete m_model;
    }

    static int main(int argc, char* argv[]) {
        srand(time(nullptr));
        QApplication app(argc, argv);
        GLTest widget;
        widget.show();
        return app.exec();
    }

    typedef void (GLTest::*on_key)(int);
    std::map<int, on_key> key_press = {
        {Qt::Key_Escape, &GLTest::exit_program},
        {Qt::Key_Left, &GLTest::less},
        {Qt::Key_Right, &GLTest::more},

        {Qt::Key_A, &GLTest::move_l},
        {Qt::Key_D, &GLTest::move_r},
        {Qt::Key_W, &GLTest::move_u},
        {Qt::Key_S, &GLTest::move_d},
        {Qt::Key_Q, &GLTest::move_f},
        {Qt::Key_E, &GLTest::move_b},
    };

    void exit_program(int) { close(); }

    void move_l(int) {
        vec3f cam_dir = normalize(cam_pos - cam_tar);
        vec3f right = normalize(cross(cam_dir, vec3f(0,1,0)));
        cam_pos += rts*right;
        updateCam();
    }

    void move_r(int) {
        vec3f cam_dir = normalize(cam_pos - cam_tar);
        vec3f right = normalize(cross(cam_dir, vec3f(0,1,0)));
        cam_pos -= rts*right;
        updateCam();
    }

    void move_u(int) {
        vec3f cam_dir = normalize(cam_pos - cam_tar);
        vec3f right = normalize(cross(cam_dir, vec3f(0,1,0)));
        vec3f cam_up = normalize(cross(right, cam_dir));

        vec3f new_pos = cam_pos + cam_up;
        vec3f new_dir = normalize(new_pos - cam_tar);
        float new_cos = dot(new_dir, vec3f(0,1,0));
        if (std::fabs(new_cos) < 0.9f) {
            cam_pos = new_pos;
            updateCam();
        }
    }

    void move_d(int) {
        vec3f cam_dir = normalize(cam_pos - cam_tar);
        vec3f right = normalize(cross(cam_dir, vec3f(0,1,0)));
        vec3f cam_up = normalize(cross(right, cam_dir));

        vec3f new_pos = cam_pos - cam_up;
        vec3f new_dir = normalize(new_pos - cam_tar);
        float new_cos = dot(new_dir, vec3f(0,1,0));
        if (std::fabs(new_cos) < 0.9f) {
            cam_pos = new_pos;
            updateCam();
        }
    }

    void move_f(int) {
//        vec3f cam_dir = normalize(cam_pos - cam_tar);
//        cam_pos += cam_dir;
//        updateCam();
    }

    void move_b(int) {
//        vec3f cam_dir = normalize(cam_pos - cam_tar);
//        cam_pos -= cam_dir;
//        updateCam();
    }

    void updateCam() {
        cam = lookat(cam_pos, cam_tar, vec3f(0,1,0));
        m_filter->setMat(cam);
    }

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
    }

    virtual void resizeGL(int w, int h) {
        m_filter->resize(w, h);
    }

    virtual void paintGL() {
        m_filter->render();
        draw_text("Hello kittie!");
    }

    void draw_text(const std::string& s) {
        glColor3f(1, 1, 1);
        QString text = QString(s.c_str());//.arg(mValue);
        //int length = QFontMetrics(font()).width(text);
        //renderText(width() - length - 1, font().pointSize() + 1, text);
        renderText(1, font().pointSize() + 1, text);
    }

private:
    bacchus::Image m_img;
    int m_width, m_height;
    bacchus::Texture* m_tex;
    bacchus::FilterModel3d* m_filter;
    bacchus::Model* m_model;
    float m_data;
    mat4f cam;
    vec3f cam_pos;
    vec3f cam_tar;
    float rts = 1.0f;
};

int main(int argc, char* argv[]) {
    return GLTest::main(argc, argv);
}
