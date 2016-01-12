# image
image manipulations  

- image.h: struct for storing image data
- png/tga r/w: simple reading/writing different image formats (png and tga)  
  png depends on libpng, add it to linker, includes png.h  
  tga based on https://github.com/ssloy/tinyrenderer tgaimage
- paint: drawing lines, circles other...  
    line, traingle from this tutorial: http://habrahabr.ru/post/248153 
    mandelbrot based on http://www.iquilezles.org by Inigo Quilez
    font: text writing based on ??? 
    xkcd lines based on http://habrahabr.ru/post/269931
- plot: drawing XY data
- arthops, distance, edgedetect and other based on http://homepages.inf.ed.ac.uk/rbf/HIPR2/wksheets.htm

