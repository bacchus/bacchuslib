#include "setting.h"

#include "image/image.h"
#include "math/fft.h"
#include "math/matalg.h"

using namespace bacchus;

TEST(GaussInverse, test) {
    if (0) {
        // test big gauss
        float f16[] = {
            1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,
            1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,
            0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,
            0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,
            1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,
            0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0,
            0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,
            0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,
            0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,
            0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,
            0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,
            1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,
            0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,
            0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,
            1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,
        };// div 5

        float f4[] = {
             2,  0,  2, 0.6,
             3,  3,  4,  -2,
             5,  5,  4,   2,
            -1, -2,3.4,  -1
        };



        matxf gaus(4,4,f4);
//        std::cout<<"min: "<<gaus.determinant_minor()<<std::endl<<std::endl;
//        std::cout<<"lu : "<<gaus.determinant_lu()<<std::endl<<std::endl;
        std::cout<<"lup: "<<determinant(gaus)<<std::endl<<std::endl;


        const int n = 4;
        matxf l(n,n);
        matxf u(n,n);
        matxf p(n,n);

        matxf m1(gaus);
        std::cout << "A\n" << m1 << std::endl;

        std::vector<int> t(n);
        decomposition_lup(m1,t);


        std::cout << "L\n" << l << std::endl;
        std::cout << "U\n" << u << std::endl;
        std::cout << "P\n" << p << std::endl;

        matxf pa = p*m1;
        std::cout << "P*A\n" << pa << std::endl;
        matxf lu = l*u;
        std::cout << "L*U\n" << lu << std::endl;

        assert(pa == lu);









        float finv[] = {
            13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3,
            3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7,
            -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3,
            3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2,
            -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12,
            -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3, 8,
            8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2, 3,
            3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3, -2,
            -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8, 3,
            3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12, 8,
            8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2, -12,
            -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3, -2,
            -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7, 3,
            3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3, -7,
            -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13, 3,
            3, -7, 3, -2, -12, 8, 3, -2, 3, 8, -12, -2, 3, -7, 3, 13,
        };// div 15

        float fimg[] = {
            2,4,5,3,
            7,6,3,4,
            1,2,0,8,
            3,7,9,1
        };

        if (0) {
            matxf img(16,1,fimg);
            matxf imt1 = transpose(img);
            //imt1.test_print();

            matxf gaus(16,16,f16);
            gaus /= 5;

            matxf blured_img = gaus*img;
            matxf imt2 = transpose(blured_img);
            //imt2.test_print();

            matxf revg(16,16,finv);//= inverse(m1);
            revg /= 3;

            matxf back = revg*blured_img;
            matxf bckt = transpose(back);
            //bckt.test_print();

            matxf one = revg*gaus;
            //one.test_print();
        }

    }




    if (0) {
        // test inverse gauss
        const int w = 10;
        float fimg[w] = {2,5,3,4,8, 1,6,7,3,4};

        // 1  -2   1   1 |-2|  4  -2  -2   4  -2
        float fgauss1[w*w] = {
            2,1,0,0,0,0,0,0,0,0,
            1,1,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,0,
            0,0,1,1,1,0,0,0,0,0,
            0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,1,1,0,0,0,
            0,0,0,0,0,1,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,
            0,0,0,0,0,0,0,1,1,1,
            0,0,0,0,0,0,0,0,1,2
        };

        // 0   0   0   0  |0|  3  -3   0   3  -2
        float fgauss2[w*w] = {
            1.5f,1.5f,0,0,0,0,0,0,0,0,
            1,1,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,0,
            0,0,1,1,1,0,0,0,0,0,
            0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,1,1,0,0,0,
            0,0,0,0,0,1,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,
            0,0,0,0,0,0,0,1,1,1,
            0,0,0,0,0,0,0,0,1.5f,1.5f
        };

        // 1   1  -2   1  |1|  1  -2   1   1  -2
        float fgauss3[w*w] = {
            1,1,0,0,0,0,0,0,0,1,
            1,1,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,0,
            0,0,1,1,1,0,0,0,0,0,
            0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,1,1,0,0,0,
            0,0,0,0,0,1,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,
            0,0,0,0,0,0,0,1,1,1,
            1,0,0,0,0,0,0,0,1,1
        };

        // 0   0   0   0  |0|  3  -3   0   3  -3
        float fgauss4[w*w] = {
            1,1,0,0,0,0,0,0,0,0,
            1,1,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,0,
            0,0,1,1,1,0,0,0,0,0,
            0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,1,1,0,0,0,
            0,0,0,0,0,1,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,
            0,0,0,0,0,0,0,1,1,1,
            0,0,0,0,0,0,0,0,1,1
        };

        // degenerate matrix
        float fgauss5[w*w] = {
            1,0,0,0,0,0,0,0,0,0,
            1,1,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,0,
            0,0,1,1,1,0,0,0,0,0,
            0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,1,1,0,0,0,
            0,0,0,0,0,1,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,
            0,0,0,0,0,0,0,1,1,1,
            0,0,0,0,0,0,0,0,0,1
        };

        matxf gauss1(w,w,fgauss1);
        matxf gauss2(w,w,fgauss2);
        matxf gauss3(w,w,fgauss3);
        matxf gauss4(w,w,fgauss4);
        matxf gauss5(w,w,fgauss5);
        gauss1.osprint(std::cout);
        gauss2.osprint(std::cout);
        gauss3.osprint(std::cout);
        gauss4.osprint(std::cout);
        gauss5.osprint(std::cout);

        matxf img(w,fimg);
        matxf gauss(w,w,fgauss3);
        gauss /= 3.0f;
        matxf blured = gauss*img;

        transpose(img).osprint(std::cout);
        transpose(blured).osprint(std::cout);

        blured[w/2] += 1.0f;// algorithm very sensitive to noise

        matxf unblured(w);

        unblured = solve_system(gauss, blured);

        transpose(unblured).osprint(std::cout);

        matxf sharp = inverse(gauss);

        sharp.osprint(std::cout);

        matxf simg = sharp*blured;
        transpose(simg).osprint(std::cout);

        // test degenerate small
        if (0) {
            const int w = 4;
            float fgauss1[] = {
                1,0,0,0,
                1,1,1,0,
                0,1,1,1,
                0,0,0,1
            };
            matxf g(w,w,fgauss1);
            g.osprint(std::cout);
            std::cout<<"det: "<<determinant(g)<<std::endl;
            inverse(g).osprint(std::cout);
        }
    }

}
