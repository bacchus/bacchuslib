#include <iostream>
#include <string>

#include <GL/glew.h>
//#include <GLES3/gl3.h>
//#include <GL/gl.h>

#include <QDebug>
#include <QApplication>
#include <QGLWidget>
#include <QImage>
#include <QKeyEvent>
#include <QTime>

#define BCC_ERROR_GL(msg)

namespace bacchus {

class GLabShader {
public:
    GLabShader()
        : mVsid(0), mFsid(0), mProid(0),
          mVSsrc(""), mFSsrc("")
    {}

    GLabShader(const std::string& vsrc, const std::string& fsrc)
        : mVsid(0), mFsid(0), mProid(0),
          mVSsrc(""), mFSsrc("")
    {
        init(vsrc, fsrc);
    }

    ~GLabShader() {
        if (mProid) {
            if (mVsid) {
                glDetachShader(mProid, mVsid);
                glDeleteShader(mVsid);
            }
            if (mFsid) {
                glDetachShader(mProid, mFsid);
                glDeleteShader(mFsid);
            }
            glDeleteProgram(mProid);
        }
    }

    void init(const std::string& vsrc, const std::string& fsrc) {
        mVSsrc = vsrc;
        mVsid = glCreateShader(GL_VERTEX_SHADER);
        if (!mVsid) {
            BCC_ERROR_GL("fail to create vertex shader");
        }
        initShader(mVsid, mVSsrc);

        mFSsrc = fsrc;
        mFsid = glCreateShader(GL_FRAGMENT_SHADER);
        if (!mFsid) {
            BCC_ERROR_GL("fail to create fragment shader");
        }
        initShader(mFsid, mFSsrc);

        mProid = glCreateProgram();
        if (!mProid) {
            BCC_ERROR_GL("fail to create program");
        }
        glAttachShader(mProid, mVsid);
        glAttachShader(mProid, mFsid);

        glLinkProgram(mProid);
        GLint status;
        glGetProgramiv(mProid, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            BCC_ERROR_GL("fail to link program\n" + logInfoProgram());
        }
    }

private:
    void initShader(GLuint id, const std::string& src) {
        const char* srcstr = src.c_str();
        glShaderSource(id, 1, &srcstr, NULL);
        glCompileShader(id);
        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            BCC_ERROR_GL("fail to compile shader\n" + logInfoShader());
        }
    }

    std::string logInfoShader(int id) {
        std::string logstr;
        int infoLength = 0;
        int charsWritten  = 0;
        char* infoLog;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0) {
            infoLog = new char[infoLength];
            glGetShaderInfoLog(id, infoLength, &charsWritten, infoLog);
            logstr += "Shader log: ";
            logstr += infoLog;
            delete[] infoLog;
        }
        return logstr;
    }

    std::string logInfoProgram(int id) {
        std::string logstr;
        int infoLength = 0;
        int charsWritten  = 0;
        char* infoLog;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0) {
            infoLog = new char[infoLength];
            glGetProgramInfoLog(id, infoLength, &charsWritten, infoLog);
            logstr += "Program log: ";
            logstr += infoLog;
            delete[] infoLog;
        }
        return logstr;
    }

private:
    GLuint mVsid;
    GLuint mFsid;
    GLuint mProid;
    std::string mVSsrc;
    std::string mFSsrc;
};

class GLabFps {
public:
    GLabFps() {
        mCounter = 0;
        mValue = 0;
        mLastPeriod = 0;
    }

    void start(QGLWidget* widget, int interval) {
        mTimer.start();
        mLastPeriod = mTimer.elapsed() - 1000;
        widget->startTimer(interval);
    }

    void draw(QGLWidget* widget) {
        int t = mTimer.elapsed();
        if (t - mLastPeriod > 1000) {
            mValue = mCounter;
            mCounter = 0;
            mLastPeriod = t;
        }
        mCounter++;
        QString text = QString("%1 fps").arg(mValue);
        int length = QFontMetrics(widget->font()).width(text);

        //        glUseProgram(0);
        //        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //        glDisable(GL_LIGHTING);
        //        glDisable(GL_TEXTURE_2D);
        //glColor3f(1, 1, 1);
        glClearColor(0.7f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        widget->renderText(widget->width() - length - 1, widget->font().pointSize() + 1, text);
    }

private:
    int mCounter;     // counter for this period
    int mValue;       // dispalyed to the used
    int mLastPeriod;  // time point of last update
    QTime mTimer;
};

class GLabGlWidget: public QGLWidget {
public:
    GLabGlWidget(QWidget* parent = 0)
        : QGLWidget(parent)
    {
        setAttribute(Qt::WA_QuitOnClose);
    }

    ~GLabGlWidget() {}

protected:
    virtual void timerEvent(QTimerEvent* /*event*/) {
        updateGL();
    }

    virtual void keyPressEvent(QKeyEvent* event) {
        int key = event->key();
        if (key == Qt::Key_Escape)
            close();

        //        switch (key) {
        //        case Qt::Key_W:
        //            // key handle
        //            break;
        //        }
    }

    virtual void mouseMoveEvent(QMouseEvent* /*event*/) {
        //        int deltaX = event->x() - mLastMousePosition.x();
        //        int deltaY = event->y() - mLastMousePosition.y();
        //        int buttons = event->buttons();
        //        if (buttons & Qt::RightButton) {
        //        } else if (buttons & Qt::MidButton) {
        //        } else if(buttons & Qt::LeftButton) {
        //        }
        //        // save last position
        //        mLastMousePosition = event->pos();
    }

    virtual void updateGL() {
        QGLWidget::updateGL();
        //paintGL();
    }

    virtual void resizeGL(int w, int h) {
        mViewportSize = QSize(w, h);
    }

    virtual void initializeGL() {
        mFps.start(this, 10);
        getDriverInfoString();
    }

    virtual void paintGL() {
        glViewport(0, 0, mViewportSize.width(), mViewportSize.height());
        mFps.draw(this);
    }

    void getDriverInfoString() const {
        if(!context()->isValid())
            std::cout << "<invalid OpenGL context>" << std::endl;

        // read OpenGL infos
        std::cout << "Vendor:   " << (const char*)glGetString(GL_VENDOR)   << std::endl;
        std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version:  " << (const char*)glGetString(GL_VERSION)  << std::endl;
        std::cout << std::endl;

        // print framebuffer format
        QGLFormat fmt = context()->format();
        std::cout << "Framebuffer Format:" << std::endl;
        std::cout << " RGBA bits:    "
                  << "(" << fmt.redBufferSize()
                  << "," << fmt.greenBufferSize()
                  << "," << fmt.blueBufferSize()
                  << "," << fmt.alphaBufferSize()
                  << ")" << std::endl;
        std::cout << " Depth bits:   " << fmt.depthBufferSize() << std::endl;
        std::cout << " Stencil bits: " << fmt.stencilBufferSize() << std::endl;
        std::cout << std::endl;

        // shading language version
        if (fmt.openGLVersionFlags() & fmt.OpenGL_Version_2_0) {
            std::cout << "Shading Language Version: "
                      << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)
                      << std::endl;
        }

        std::cout << std::endl;

        GLint i;
#define PRINT_CONSTANT(Name) \
    glGetIntegerv(Name, &i); \
    std::cout << #Name " = " << i << std::endl;

        PRINT_CONSTANT(GL_MAX_TEXTURE_SIZE);
        PRINT_CONSTANT(GL_MAX_TEXTURE_UNITS);
        PRINT_CONSTANT(GL_MAX_VERTEX_ATTRIBS);
        PRINT_CONSTANT(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
        PRINT_CONSTANT(GL_MAX_VARYING_FLOATS);
        PRINT_CONSTANT(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        PRINT_CONSTANT(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
        PRINT_CONSTANT(GL_MAX_TEXTURE_IMAGE_UNITS);
        PRINT_CONSTANT(GL_MAX_TEXTURE_COORDS);
        PRINT_CONSTANT(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
#undef PRINT_CONSTANT

    }


private:
    GLabFps mFps;
    QSize mViewportSize;
    //QPoint mLastMousePosition;
};

} // namespace bacchus

int main(int argc, char* argv[]) {
    srand(time(NULL));
    QApplication app(argc, argv);
    bacchus::GLabGlWidget win;
    QFont font("Courier", 12);
    win.setFont(font);
    win.show();
    win.move(200, 100);
    return app.exec();
}
