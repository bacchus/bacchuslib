#pragma once

#include <QGLWidget>
#include <QImage>

namespace bacchus {

// use of qt image
inline void test_qimage(const char* imgName) {
    QImage mImage;
    mImage = QGLWidget::convertToGLFormat(QImage(imgName));
    if(mImage.isNull()) {
        std::cout << "no valid image given" << std::endl;
        return;
    }
    unsigned char* bits = mImage.bits();
    int width = mImage.width();
    int height = mImage.height();
}

} // namespace bacchus
