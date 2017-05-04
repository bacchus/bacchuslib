#include <vector>
#include <limits>

#include <QApplication>
#include <QGLWidget>
#include <QKeyEvent>

#include "audio/audio_tools.h"

#include "glwrap/program.h"
#include "glwrap/utilsgl.h"

#include "image/png_rw.h"
#include "image/tga_rw.h"
#include "utils/timer.h"

#include "math/mdefs.h"
#include "math/transform.h"
#include "math/fft.h"

#include "cvml/linear_regression.h"
#include "cvml/logistic_regression.h"

#include "filters/filter_simple.h"
#include "filters/filter_gauss.h"
#include "filters/filter_lighting.h"

#include "image/plot.h"
#include "image/paint.h"
#include "image/arthops.h"
#include "image/histogram.h"
#include "image/geometric.h"
#include "image/morphology.h"
#include "image/filters_smooth.h"
#include "image/fourier.h"
#include "image/distance.h"
#include "image/edgedetect.h"
#include "image/hough.h"

#include "raytrace/obj_model.h"
#include "raytrace/raytracer_devmaster.h"
#include "raytrace/soft_renderer.h"
#include "raytrace/soft_renderer_shaders.h"

const float g_start_param = 1.0f;
const float g_delta_param = 0.1f;
const float g_min_param = 0.0f;
const float g_max_param = 1.0f;

using namespace bacchus;

class GLTest: public QGLWidget {
public:

    /// raytracing
    vec3f eye = vec3f(-1,-1,20);
    vec3f tar = vec3f(0,0,0);
    float rts = 0.5f;
    Raytracer* m_tracer;
    void raytrace() {
        Timer timer;
        timer.start();
        m_oimg[0].clear();
        m_tracer = new Raytracer(m_oimg[0]);
        m_tracer->scene.init();
        m_tracer->init_render(eye,tar);
        m_tracer->render();
        Geometric::flip_vert(m_oimg[0], m_oimg[0]);
        char infostr[64];
        sprintf(infostr, "time: %.0f ", timer.ms());
        Paint paint(m_oimg[0]);
        paint.draw_text(infostr, vec2i(0, m_oimg[0].height-20), lum2i(255));
//        sprintf(infostr, "eye:(%.0f,%.0f,%.0f)", eye.x, eye.y, eye.z);
//        paint.draw_text(infostr, vec2i(0, m_oimg[0].height-40), lum2i(255));
//        sprintf(infostr, "tar:(%.0f,%.0f,%.0f)", tar.x, tar.y, tar.z);
//        paint.draw_text(infostr, vec2i(0, m_oimg[0].height-60), lum2i(255));
        static bool inited = false;
        if (inited) {
            m_tex->pixels();
            m_filter->setChanged();
        } else {
            inited = true;
        }
    }

    GLTest(QWidget* parent = 0)
        : QGLWidget(parent)
    {
        LOGD("Hello kittie!");

        /// laod images
//        for (int i = 0; i < 9; ++i) {
//            m_oimg[i] = m_img;
//            oimg_name[i] = "xz";
//        }
//        oimg_name[9] = "original";
        bacchus::read_png_mirrored("test.png", m_img);
        bacchus::read_png_mirrored("light/light_img.png", m_oimg[0]);
        bacchus::read_png_mirrored("light/light_img.png", m_oimg[6]);
        bacchus::read_png_mirrored("light/light_img.png", m_oimg[7]);

        m_img.to_a1().to_gray();
        m_oimg[3] = m_img;
        m_oimg[4] = m_img;
        //Geometric::scale_up_size(m_img, m_oimg[3], m_img.width+220, m_img.height+50, 0, 50, Geometric::FILTER_LINEAR);
        m_oimg[0] = m_oimg[1] = m_oimg[2] = m_oimg[3] = m_img;
        m_oimg[0].clear().to_a1();//.to_gray();

        //====================================================================
        /// FFT test
        if (0) {
            int fs = 10000;
            int f1 = 80;
            matxf x1 = genSine(1,f1,0,fs,1024);
            matxcx y = fft(x1);
//            matxcx y = DFT(x1);
            matxf mag = genMagSpecDb_fromdft(y);

            Plot plot(m_oimg[0]);
            plot.plotData(mag, Paint::green);
        }

        //====================================================================
        /// perlin noise
        if (0) {
            Paint paint(m_oimg[0]);
            paint.draw_perlin_noise();
        }

        //====================================================================
        /// handwriten curves
        if (0) {
            Paint paint(m_oimg[0]);
            vec2f o(50,0.5f*paint.resolution().y);
            vec2f sz = 0.8f*(vec2f)paint.resolution() - o;

            paint.draw_curve({vec2i(10,o.y), vec2i(1.2f*sz.x,o.y)}, Paint::green);
            paint.draw_curve({vec2i(o.x,100), vec2i(o.x,3*sz.y)}, Paint::red);

//            paint.draw_bezier_curve(u
//                        vec2i(10,o.y)
//                        , vec2i(o.x + 0.25f*sz.x, o.y - 40)
//                        , vec2i(o.x + 0.75f*sz.x, o.y + 40)
//                        , vec2i(o.x + sz.x, o.y)
//                        , Paint::green);

            std::vector<vec2i> line;
            int n = 40;
            for (int i = 0; i < n+1; ++i) {
                float x1 = (float)i/(float)n;
                float y1 = std::sin(2*M_PI*x1);
                line.push_back(sz*vec2f(x1,y1) + o);
            }
            paint.draw_curve(line, Paint::white);
        }

        //====================================================================
        /// plot x-y data
        if (0) {
            matxf x;
            matxf y;
            load_data("ex1data1.txt", x,y);
            Plot plot(m_oimg[0]);
            plot.plotData(ext_col_get(x,1),y, Paint::red);
        }

        //====================================================================
        /// raytracer
        if (0) {
            raytrace();
        }

//        Smooth::gauss_2r(m_img, m_oimg[0]);
//        Smooth::sharpen3(m_oimg[0], m_img);
//        Edgedetect::sobel_2wrap(m_img, m_oimg[0]);
//        Histogram::threshold_constant(m_oimg[0], m_img, 128);
//        Morphology::thinn(m_oimg[1], m_oimg[0]);

        //====================================================================
        /// 3d renderer shader
        if (1) {
            //TODO: crashes if image not initialized
            m_oimg[0].clear().to_a1();//.to_gray();
            Model model("model.obj");
            bacchus::read_tga_mirrored("light/diffuse.tga", m_oimg[3]);
            bacchus::read_tga_mirrored("light/norm.tga", m_oimg[4]);
            bacchus::read_tga_mirrored("light/spec.tga", m_oimg[5]);
            //Model model("floor/floor.obj");
            //bacchus::read_tga_mirrored("floor/diffuse.tga", m_oimg[3]);
            int width = m_img.width;
            int height = m_img.height;
            int depth = 255;
            mat4f depth_mvp;
            int shadowbuffer[width*height];//Image zbuffer(width,height);
            memset(shadowbuffer, 0, width*height*sizeof(int));
            {
                /// draw lighting depth
                Renderer3d renderer(m_oimg[6]);
                vec3f light_dir(1,1,1); light_dir.normalize();
                vec3f eye(1,1,3);
                vec3f center(0,0,0);
                vec3f up(0,1,0);
                renderer.set_modelview(light_dir, center, up);
                renderer.set_viewport(width/8.f,height/8.f, width*3.f/4.f, height*3.f/4.f, depth);
                renderer.set_projection(0);
                depth_mvp = renderer.mvp();
                DepthShader shader(model, depth_mvp);
                for (int i = 0; i < model.nfaces(); ++i) {
                    vec4f scr[3];
                    for (int j = 0; j < 3; ++j) {
                        scr[j] = shader.vertex(i,j);
                    }
                    renderer.triangle(scr, shader, shadowbuffer);
                }
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        int idx = x+y*width;
                        m_oimg[7].pix(x,y) = lum2i(clamp_color(shadowbuffer[idx]));
                    }
                }
                //write_tga_mirrored("depth1.tga", m_oimg[7]);
            }
            Renderer3d renderer(m_oimg[0]);
            vec3f light_dir(1,1,1); light_dir.normalize();
            vec3f eye(1,1,3);
            vec3f center(0,0,0);
            vec3f up(0,1,0);
            int zbuffer[width*height];//Image zbuffer(width,height);
            memset(zbuffer, 0, width*height*sizeof(int));
            renderer.set_modelview(eye, center, up);
            renderer.set_viewport(width/8.f,height/8.f, width*3.f/4.f, height*3.f/4.f, depth);
            renderer.set_projection(-1.f/(eye-center).length());
            //ShadowShader shader(model, renderer.mvp(), m_oimg[3], depth_mvp, shadowbuffer);
            //SimpleShader shader(model, renderer.mvp(), m_oimg[3]);
            //GouraudShader shader(model, renderer.mvp(), light_dir, m_oimg[3]);
            NormMapingShader shader(model, renderer.mvp(), light_dir, eye,
                                    m_oimg[3], m_oimg[4], m_oimg[5]
                    , depth_mvp, shadowbuffer);// shadow
            for (int i = 0; i < model.nfaces(); ++i) {
                vec4f scr[3];
                for (int j = 0; j < 3; ++j) {
                    scr[j] = shader.vertex(i,j);
                }
                renderer.triangle(scr, shader, zbuffer);
                //vec4i col(0);
                //shader.fragment(vec3f(0.3f,0.3f,0.4f), col);
                //renderer.triangle_wire(scr, col);
            }
        }

        //====================================================================
        /// Paint text
        if (0) {
            Paint paint(m_oimg[0]);
            paint.draw_text("Hello World! =)", vec2i(100,600), rgba2i(250, 250, 80));
            const char* test_str = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
            paint.draw_text(test_str, vec2i(100,500), lum2i(255));
            paint.draw_text("2+3*7=19; sag,dth, drry", vec2i(100,300), rgba2i(250, 40, 80));
        }

        //====================================================================
        /// find normals
//         vec3f n = cross(world[2] - world[0], world[1] - world[0]).normalize();
        /// save norm map to image
//        vec3f norm_rgb = 0.5f*255.f*(n + vec3f(1.f));
//        pnorm.draw_triangle(scr[0], scr[1], scr[2], rgba2i(norm_rgb.x, norm_rgb.y, norm_rgb.z), zbuffer);
        /// draw zbuffer
//        int zmax = 0;
//        int zmin = 255;
//        for (int y = 0; y < height; ++y) {
//            for (int x = 0; x < width; ++x) {
//                int idx = x+y*width;
//                zmin = std::min(zmin, zbuffer[idx]);
//                zmax = std::max(zmax, zbuffer[idx]);
//            }
//        }
//        std::cout<<zmin<<" "<<zmax<<std::endl;
//        for (int y = 0; y < height; ++y) {
//            for (int x = 0; x < width; ++x) {
//                int idx = x+y*width;
//                m_oimg[6].pix(x,y) = lum2i(255.f*(zbuffer[idx] - zmin)/(zmax - zmin));
//            }
//        }

        //====================================================================
        /// nice mandelbrought
        if (0) {
            Paint paint(m_oimg[0]);
            paint.draw_fractal_mandelbrot();

//            Histogram::threshold_adaptive(m_oimg[1], m_oimg[0], 10);
//            Morphology::dilate(m_oimg[2], m_oimg[1], 2);
//            Morphology::thinn(m_oimg[1], m_oimg[2]);
//            Arthops::logic_or(m_oimg[0], m_oimg[0], m_oimg[1]);
        }

        //====================================================================
        /// draw lines in hough transform
        if (0) {
            std::vector<vec2f> res = Hough::transform(m_oimg[3], m_oimg[1]);
            oimg_name[0] = "hough";
            Paint p5(m_oimg[2]);
            for (uint i = 0; i < res.size(); ++i) {
                float si = sin(res[i].x);
                float co = cos(res[i].x);
                float ro = res[i].y;
                int wi = m_img.width;
                int he = m_img.height;
                if (eq(si, 0.f)) {
                    p5.draw_line(vec2i(ro,0), vec2i(ro,he), Paint::orange);
                } else {
                    float y0 = ro/si;
                    float yw = y0 - wi*co/si;
                    p5.draw_line(vec2i(0,y0), vec2i(wi,yw), Paint::orange);
                }
                res[i].x = rad2deg(res[i].x);
                std::cout<<res[i]<<"; ";
            }
            std::cout<<std::endl;
        }

        //====================================================================
        /// setup qt window
        setWindowTitle("Hello kittie");
        setAttribute(Qt::WA_QuitOnClose);
        startTimer(10);
        m_width = m_img.width;
        m_height = m_img.height;
        resize(m_width, m_height);

        /// openg specific
        curoimg = 0;
        m_tex = new bacchus::Texture(m_width, m_height, m_oimg[curoimg].dat8);
        //m_tex1 = new bacchus::Texture(m_width, m_height, m_oimg[1].dat8);
        m_filter = new bacchus::FilterSimple();
//        m_filter = new bacchus::FilterLighting();
        m_filter->resize(m_width, m_height);
        m_filter->setInput(m_tex);
        m_filter->setOutput(NULL);
//        m_filter->addTexture("img_norm", m_tex1);
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

        {Qt::Key_A, &GLTest::move_l},
        {Qt::Key_D, &GLTest::move_r},
        {Qt::Key_W, &GLTest::move_u},
        {Qt::Key_S, &GLTest::move_d},
        {Qt::Key_Q, &GLTest::move_f},
        {Qt::Key_E, &GLTest::move_b},

//        {Qt::Key_Left,  &GLTest::look_l},
//        {Qt::Key_Right, &GLTest::look_r},
//        {Qt::Key_Up,    &GLTest::look_u},
//        {Qt::Key_Down,  &GLTest::look_d},
//        {Qt::Key_M,     &GLTest::look_f},
//        {Qt::Key_N,     &GLTest::look_b},

        {Qt::Key_1, &GLTest::change_pic},
        {Qt::Key_2, &GLTest::change_pic},
        {Qt::Key_3, &GLTest::change_pic},
        {Qt::Key_4, &GLTest::change_pic},
        {Qt::Key_5, &GLTest::change_pic},
        {Qt::Key_6, &GLTest::change_pic},
        {Qt::Key_7, &GLTest::change_pic},
        {Qt::Key_8, &GLTest::change_pic},
        {Qt::Key_9, &GLTest::change_pic},

        {Qt::Key_O, &GLTest::save_gl},

        {Qt::Key_T, &GLTest::increase_t},
    };

    void exit_program(int) { close(); }

    void move_l(int) { eye += vec3f( rts,0,0); raytrace(); }
    void move_r(int) { eye += vec3f(-rts,0,0); raytrace(); }
    void move_u(int) { eye += vec3f(0, rts,0); raytrace(); }
    void move_d(int) { eye += vec3f(0,-rts,0); raytrace(); }
    void move_f(int) { eye += vec3f(0,0, rts); raytrace(); }
    void move_b(int) { eye += vec3f(0,0,-rts); raytrace(); }

    void save_gl(int) {
        m_oimg[2].bytespp = Image::FMT_RGBA;
        savegl(m_oimg[2].dat8, m_oimg[2].width, m_oimg[2].height);
        write_png_mirrored("glsave.png", m_oimg[2]);
    }

//    void look_l(int) { tar += vec3f( rts,0,0); raytrace(); }
//    void look_r(int) { tar += vec3f(-rts,0,0); raytrace(); }
//    void look_u(int) { tar += vec3f(0, rts,0); raytrace(); }
//    void look_d(int) { tar += vec3f(0,-rts,0); raytrace(); }
//    void look_f(int) { tar += vec3f(0,0, rts); raytrace(); }
//    void look_b(int) { tar += vec3f(0,0,-rts); raytrace(); }

    void change_pic(int key) {
        curoimg = key - Qt::Key_1;
        m_tex->pixels(m_img.width, m_img.height, m_oimg[curoimg].dat8);
        m_filter->setChanged();
    }

    void increase_t(int) {
        m_t += 0.01f;
        Paint paint(m_oimg[0]);
        paint.draw_perlin_noise(m_t);
        m_tex->pixels(m_img.width, m_img.height, m_oimg[curoimg].dat8);
        m_filter->setChanged();
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
        //m_tex1->createGl();
        //m_tex1->pixels();
        m_filter->init();
    }

    virtual void resizeGL(int w, int h) {
        m_filter->resize(w, h);
    }

    virtual void paintGL() {
        m_filter->render();
        draw_text(oimg_name[curoimg]);
//        draw_hist(m_hist_r, 3);
//        draw_hist(m_hist_g, 1);
//        draw_hist(m_hist_b, 2);
    }

    void draw_text(const std::string& s) {
        QString text = QString(s.c_str());//.arg(mValue);
        int length = QFontMetrics(font()).width(text);
        glColor3f(1, 1, 1);
        renderText(width() - length - 1, font().pointSize() + 1, text);
    }

    void draw_hist(const std::vector<float>& hist, int f, float d=1.0f) {
        if (f==0) { glColor3f(1, 0, 0);}
        else if (f==1) { glColor3f(0, 1, 0);}
        else if (f==2) { glColor3f(0, 0, 1);}
        else if (f==3) { glColor3f(0, 0, 0);}

        for (int i = 1; i < 255; ++i) {
            float h = (d - 0.75f*hist[i])*height();
            renderText(i*width()/256, h, "-");
            if (i == (int)hist[0]) {
                renderText(i*width()/256, h-10, "|");
            }
        }
    }

private:
    //png_image m_img;
    std::vector<float> m_hist_r;
    std::vector<float> m_hist_g;
//    std::vector<float> m_hist_b;
    //bacchus::Image m_oimg;
    bacchus::Image m_oimg[10];
    std::string oimg_name[10];
    uint curoimg;
    bacchus::Image m_img;
    int m_width, m_height;
    bacchus::Texture* m_tex;
    //bacchus::Texture* m_tex1;
    bacchus::FilterSimple* m_filter;
//    bacchus::FilterLighting* m_filter;
    float m_data;
    float m_t = 0.8f;
};

int main(int argc, char* argv[]) {
    return GLTest::main(argc, argv);
}
