#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>

#include "thread/thread.h"

using namespace std;
using namespace bacchus;


// Mandelbrot fractal settings
#define MAND_MID_RE -0.69795
#define MAND_MID_IM -0.34865
#define MAND_SIZE 0.001
#define MAND_MAX_ITER 10000
#define MAND_RESOLUTION 1024


/// BGR pixel class.
class Pixel {
public:
    Pixel() : b(0), g(0), r(0) {}

    Pixel(unsigned char red, unsigned char green, unsigned char blue) {
        b = blue;
        g = green;
        r = red;
    }

    unsigned char b, g, r;
};

/// Simple 2D BGR image class.
class Image {
public:
    /// Create a new image with the dimensions aWidth x aHeight.
    Image(int aWidth, int aHeight) {
        mData = new Pixel[aWidth*aHeight];
        mWidth = aWidth;
        mHeight = aHeight;
    }

    ~Image() {
        delete mData;
    }

    /// Write the image to a TGA file.
    void WriteToTGAFile(const char * aFileName)
    {
        // Prepare TGA file header
        unsigned char header[18] = {0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,24,0};
        header[12] = mWidth & 255;         // Image width (16 bits)
        header[13] = (mWidth >> 8) & 255;  // -"-
        header[14] = mHeight & 255;        // Image width (16 bits)
        header[15] = (mHeight >> 8) & 255; // -"-

        // Write output file
        ofstream f(aFileName, ios_base::out | ios_base::binary);
        f.write((const char*)header, 18);
        f.write((const char*)mData, mWidth*mHeight*3);
    }

    Pixel& operator[](const int idx) {
        return mData[idx];
    }

    inline int Width() const {
        return mWidth;
    }

    inline int Height() const {
        return mHeight;
    }

private:
    int mWidth;    ///< Image width.
    int mHeight;   ///< Image height.
    Pixel* mData;  ///< Data pointer.
};

/// The RowDispatcher class manages the "job pool" for the fractal
/// calculation threads.
class RowDispatcher {
public:
    RowDispatcher(Image* aImage) : mNextRow(0) {
        mImage = aImage;
    }

    Image* GetImage() {
        return mImage;
    }

    int NextRow() {
        LockGuard guard(mMutex);
        if(mNextRow >= mImage->Height())
            return -1;
        int result = mNextRow;
        ++ mNextRow;
        return result;
    }

private:
    Mutex mMutex;
    int mNextRow;
    Image* mImage;
};

// Core iteration function
Pixel Iterate(const double &cre, const double &cim, int aIterMax) {
    double zre = 0.0;
    double zim = 0.0;
    int n = 0;
    double absZSqr = 0.0;
    while((absZSqr < 4.0) && (n < aIterMax))
    {
        double tmp = zre * zre - zim * zim + cre;
        zim = 2.0 * zre * zim + cim;
        zre = tmp;
        absZSqr = zre * zre + zim * zim;
        ++ n;
    }
    if(n >= aIterMax)
        return Pixel(0,0,0);
    else
    {
        double nSmooth = n + 1 - log(log(sqrt(absZSqr))) / log(2.0);
        return Pixel(
                    (unsigned char)(128.0 - 127.0 * cos(0.02 * nSmooth + 0.3)),
                    (unsigned char)(128.0 - 127.0 * cos(0.016 * nSmooth + 1.2)),
                    (unsigned char)(128.0 - 127.0 * cos(0.013 * nSmooth + 2.6))
                    );
    }
}

// Calculation thread
void CalcThread(void * arg)
{
    RowDispatcher* dispatcher = (RowDispatcher*)arg;
    Image* img = dispatcher->GetImage();

    // Set min/max interval for the fractal set
    double xMin = MAND_MID_RE - MAND_SIZE * 0.5;
    double yMin = MAND_MID_IM - MAND_SIZE * 0.5;
    double xMax = MAND_MID_RE + MAND_SIZE * 0.5;
    double yMax = MAND_MID_IM + MAND_SIZE * 0.5;

    // Set step size (distance between two adjacent pixels)
    double xStep = (xMax - xMin) / img->Width();
    double yStep = (yMax - yMin) / img->Height();

    // Until no more rows to process...
    while(true) {
        // Get the next row to calculate
        int y = dispatcher->NextRow();

        // Done?
        if(y < 0)
            break;

        // Generate one row of the image
        Pixel* line = &(*img)[y * img->Width()];
        double cim = y * yStep + yMin;
        double cre = xMin;
        for(int x = 0; x < img->Width(); ++x) {
            *line ++ = Iterate(cre, cim, MAND_MAX_ITER);
            cre += xStep;
        }
    }
}

int main(int argc, char **argv)
{
    // Show some information about this program...
    cout << "This is a small multi threaded Mandelbrot fractal generator." << endl;
    cout << endl;
    cout << "The program will generate a " << MAND_RESOLUTION << "x" << MAND_RESOLUTION << " image, using one calculation thread per" << endl;
    cout << "processor core. The result is written to a TGA format image, \"fractal.tga\"." << endl;
    cout << endl;
    cout << "Type \"" << argv[0] << " -st\" to force single threaded operation." << endl;
    cout << endl;

    // Check arguments
    bool singleThreaded = false;
    if((argc >= 2) && (string(argv[1]) == string("-st")))
        singleThreaded = true;

    // Init image and row dispatcher
    Image img(MAND_RESOLUTION, MAND_RESOLUTION);
    RowDispatcher dispatcher(&img);

    // Determine the number of calculation threads to use
    int numThreads;
    if(!singleThreaded) {
        numThreads = Thread::procnum();
        std::cout << "procnum: " << numThreads << std::endl;
        if(numThreads < 1)
            numThreads = 1;
    } else {
        numThreads = 8;
    }

    // Start calculation threads (we run one thread on each processor core)
    cout << "Running " << numThreads << " calculation thread(s)..." << flush;
    list<Thread*> threadList;
    for(int i = 0; i < numThreads; ++ i) {
        Thread* t = new Thread(CalcThread, (void*)&dispatcher);
        threadList.push_back(t);
    }

    // Wait for the threads to complete...
    for(list<Thread*>::iterator i = threadList.begin(); i != threadList.end(); ++ i) {
        Thread* t = *i;
        t->join();
        delete t;
    }
    cout << "done!" << endl;

    // Write the final image to a file
    cout << "Writing final image..." << flush;
    img.WriteToTGAFile("fractal.tga");
    cout << "done!" << endl;
}
