#error "Do not include this file"

void img_normalize_contrast(bacchus::Image& img) {
    int h[256];
    memset(h, 0, 256*sizeof(int));
    for (int i = 0; i < img.size8(); i+=img.fmt) {
        ++h[img.dat8[i]];
    }

    int sz = img.width*img.height;
    int remap[256];
    memset(remap, 0, 256*sizeof(int));
    int sum = 0;
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        sum += h[i];
        int target = sum*255/sz;
        for (int k = j+1; k <= target; ++k) {
            remap[k] = i;
        }
        j = target;
    }

    for (int i = 0; i < img.size8(); i+=img.fmt) {
        unsigned char tmp = remap[img.dat8[i]];
        img.dat8[i  ] = tmp;
        img.dat8[i+1] = tmp;
        img.dat8[i+2] = tmp;
    }
}

void img_histogram(const bacchus::Image& img, int f, std::vector<float>& h) {
    h.resize(256, 0);
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            ++h[img.pix(j,i,f)];
        }
    }

    const int dbg = 0;
    const int start_with = 0;
    const int end_with = 256;

    float max = h[start_with];
    //int maxi = start_with;

    for (int i = 0; i < start_with; ++i) {
        h[i] = 0;
        if (dbg) std::cout << h[i] << " ";
    }

    for (int i = start_with; i < end_with; ++i) {
        if (dbg) std::cout << h[i] << " ";
        if (h[i] > max) {
            max = h[i];
//            maxi = i;
        }
    }

    for (int i = end_with; i < 256; ++i) {
        h[i] = 0;
        if (dbg) std::cout << h[i] << " ";
    }

    std::cout << std::endl;
    std::cout << "max: " << max << std::endl;

    for (int i = 0; i < 256; ++i) {
        h[i] /= max;
    }
}


// arthops
Image& add(const Image& img) { return Arthops::add(*this, img, *this); }
Image& add(uchar c) { return Arthops::add(*this, c, *this); }
Image& sub(const Image& img) { return Arthops::sub(*this, img, *this); }
Image& asub(const Image& img) { return Arthops::asub(*this, img, *this); }
Image& sub(uchar c) { return Arthops::sub(*this, c, *this); }
Image& asub(uchar c) { return Arthops::asub(*this, c, *this); }
Image& mul(const Image& img) { return Arthops::mul(*this, img, *this); }
Image& mul(float c) { return Arthops::mul(*this, c, *this); }
Image& div(const Image& img) { return Arthops::div(*this, img, *this); }
Image& blend(const Image& img, float c) { return Arthops::blend(*this, img, c, *this); }
Image& blend(const Image& img) { return Arthops::blend(*this, img, *this); }
Image& logic_not() { return Arthops::logic_not(*this, *this); }
Image& logic_and(const Image& img) { return Arthops::logic_and(*this, img, *this); }
Image& logic_or(const Image& img) { return Arthops::logic_or(*this, img, *this); }
Image& logic_xor(const Image& img) { return Arthops::logic_xor(*this, img, *this); }
Image& logarithm() { return Arthops::logarithm(*this, *this); }
Image& exponential(float b, float c) { return Arthops::exponential(*this, b, c, *this); }

// histogram based
void histogram(int* h, int f=0) const {
    memset(h, 0, 256*sizeof(int));
    for (int i = 0; i < size8(); i+=fmt) {
        ++h[dat8[i+f]];
    }
}

Image& threshold_constant(uchar t) { return Histogram::threshold_constant(*this, *this, t); }
Image& threshold_global(int c=0) { return Histogram::threshold_global(*this, *this, c); }
// methods: histogram, mean, median, (max-min)/2
Image& threshold_adaptive(int r, int c=0) {
    Image img(width, height);
    Histogram::threshold_adaptive(*this, img, r, c);
    *this = img;
    return *this;
}
Image& contrast_stretch(uchar a, uchar b) { return Histogram::contrast_stretch(*this, *this, a, b); }
Image& histogram_equalize() { return Histogram::histogram_equalize(*this, *this); }
Image& histogram_equalize_local(int r) {
    Image img(width, height);
    Histogram::histogram_equalize_local(*this,img, r);
    *this = img;
    return *this;
}

// geometric transform
Image& scale(int w, int h, Geometric::Filter filter) {
    if (w==width && h==height) {
        return *this;
    }
    Image img(w, h);
    *this = Geometric::scale(*this,img, w, h, filter);
    return *this;
}

Image& scale_up_size(int w, int h, int dx, int dy, Geometric::Filter filter) {
    Image img(width, height);
    *this = Geometric::scale_up_size(*this,img, w,h, dx,dy, filter);
    return *this;
}

Image& rotate(int x, int y, float dg) {
    Image img(width, height);
    *this = Geometric::rotate(*this,img, x,y,dg);
    return *this;
}

Image& reflect(int x, int y, float dg) {
    Image img(width, height);
    *this = Geometric::reflect(*this,img, x,y,dg);
    return *this;
}

Image& translate(int x, int y) {
    Image img(width, height);
    *this = Geometric::translate(*this,img, x,y);
    return *this;
}

Image& affine(int x10, int y10, int x11, int y11, int x12, int y12,
              int x20, int y20, int x21, int y21, int x22, int y22)
{
    Image img(width, height);
    *this = Geometric::affine(*this,img
                              ,x10,y10,x11,y11,x12,y12
                              ,x20,y20,x21,y21,x22,y22);
    return *this;
}

//TODO: void classification()// by histogram
//TODO: void connected_components()

//255: The color of the object (usually white) is referred to as the foreground color.
//0  : The rest (usually black) is referred to as the background color

// morphology
Image& morf_dilate(int r=1) {
    Image img(width, height);
    *this = Morphology::dilate(*this, img, r);
    return *this;
}

Image& morf_erode(int r=1) {
    Image img(width, height);
    *this = Morphology::erode(*this, img, r);
    return *this;
}

Image& morf_hit_miss_corners() {
    Image img(width, height);
    *this = Morphology::hit_miss_corners(*this, img);
    return *this;
}

Image& morf_open(int r=1) { return Morphology::open(*this, *this, r); }
Image& morf_close(int r=1) { return Morphology::close(*this, *this, r); }
Image& morf_thinn() { return Morphology::thinn(*this, *this); }
Image& morf_thick() { return Morphology::thick(*this, *this); }
Image& morf_skiz() { return Morphology::skiz(*this, *this); }

//TODO: Image& skeleton()
//TODO: Image& medial_axis_transform()

// Filters

Image& filter_mean(int r=1) {
    Image img(width, height);
    *this = Smooth::mean_lum(*this, img, r);
    return *this;
}

Image& filter_median(int r=1) {
    Image img(width, height);
    *this = Smooth::median_lum(*this, img, r);
    return *this;
}

Image& filter_gauss(float s=1, int r=1) {
    Image img(width, height);
    *this = Smooth::gauss_2wrap_lum(*this, img, s,r);
    return *this;
}

Image& filter_conservative(int r=1) {
    Image img(width, height);
    *this = Smooth::conservative_lum(*this, img, r);
    return *this;
}

Image& filter_crimmins(int n=1) {
    return Smooth::crimmins_lum(*this, *this, n);
}

//TODO: Image& filter_frequency


Image& filter_laplac(int eges=1) {
    Image img(width, height);
    *this = Smooth::laplac(*this, img, eges);
    return *this;
}

Image& filter_laplac_of_gauss(float s=1, int r=1) {
    Image img(width, height);
    *this = Smooth::laplac_of_gauss(*this, img, s,r);
    return *this;
}

Image& filter_unsharp() {
    float k = 0.5f;
    Image img(*this);
    img.filter_gauss(1).asub(*this).mul(k);
    add(img);
    return *this;
}

// feature detectors

Image& edgedetect_robcross() {
    Image img(*this);
    *this = Edgedetect::robcross(*this, img);
    return *this;
}

Image& edgedetect_sobel() {
    Image img(*this);
    Edgedetect::sobel(*this, img);
    *this = img;
    return *this;
}

//TODO: Image& edgedetect_canny() {

Image& edgedetect_compass() {
    Image img(*this);
    Edgedetect::compass(*this, img);
    *this = img;
    return *this;
}

Image& edgedetect_zerocross(float s=1, int r=1) {
    Image img(*this);
    Edgedetect::zerocross(*this, img, s, r);
    *this = img;
    return *this;
}

Image& detect_lines() {
    Image img(*this);
    Edgedetect::detect_lines(*this, img);
    *this = img;
    return *this;
}

Image& distance_transform() {
    return Distance::transform(*this, *this);
}

std::vector< std::vector<cplx> > fourier_transform(Image& mag, Image& arg, float& ymin, float& ymax) const {
    return Fourier::transform(*this, mag, arg, ymin, ymax);
}

Image& fourier_transform_rev(const std::vector< std::vector<cplx> >& cplximg) {
    return Fourier::inverse(cplximg, *this);
}

