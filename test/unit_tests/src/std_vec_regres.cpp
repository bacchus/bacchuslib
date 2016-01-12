//namespace glwrap {

///* linear regression model: h(x) = th0 + th1*x
// * cost func: J(th) = 0.5f/m*sum(sq(h(x[i]) - y[i]), 1, m)
// */
//std::vector<float> vec_linear_regression_one_variable(
//          const std::vector<float> &x
//        , const std::vector<float> &y
//        , float a = 0.01f               // 0.001 0.01 0.1 1 10 ...
//        , uint max_steps = 1500         // 100 1,000 10,000
//        , float presicion = 0.00001f    // 0.1 0.01 0.001 0.0001
//        )
//{
//    const uint m = x.size();
//    assert(y.size() == m);
//    std::vector<float> th(2, 0.f);

//    uint steps = 0;
//    const float adm = a/(float)m;

//    float curn_cost = 10000.f;
//    float prev_cost = curn_cost + presicion + 1.f;

//    while (prev_cost - curn_cost > presicion) {
//        prev_cost = curn_cost;
//        curn_cost = 0.f;

//        float sum[2] = {0.f, 0.f};
//        for (uint i = 0; i < m; ++i) {
//            const float isum = th[0] + th[1]*x[i] - y[i];
//            sum[0] += isum;
//            sum[1] += isum*x[i];
//            curn_cost += isum*isum;
//        }

//        th[0] -= adm*sum[0];
//        th[1] -= adm*sum[1];

//        curn_cost *= 0.5f/(float)m;
//        if (curn_cost > prev_cost) {
//            std::cout << "J is increasing: a is too large" << std::endl;
//            break;
//        }

//        ++steps;
//        if (steps > max_steps) {
//            std::cout << "slow converge: a is too small" << std::endl;
//            break;
//        }
//    }
//    return th;
//}

///*      |1  x01 x02 ..  x0N'    |
// *      |1  x11 x12             |
// * x =  |1  x21     ..          |
// *      |.. ..                  |
// *      |1  xM'1 .. ..  xM'N'   |
// *
// * (N' = N - 1, M' = M -1)
// * x - M x N
// * N - number of features
// * M - number of samples
// *
// * h0(th,x[0]) = th[0]*x[0][0] + th[1]*x[0][1] + .. + th[N]*x[0][N]
// *
// * hi(th,x[i]) = th[0]*x[i][0] + th[1]*x[i][1] + .. + th[N]*x[i][N]
// *
// * hM'(th,x[M']) = th[0]*x[M'][0] + th[1]*x[M'][1] + .. + th[N']*x[M'][N']
// *
// *
// */

//float vec_innprod(const std::vector<float>& a, const std::vector<float>& b) {
//    return std::inner_product(a.begin(), a.end(), b.begin(), 0.f);
//}

//std::vector<vec2f> vec_feature_normalize(std::vector< std::vector<float> > &x) {
//    const uint m = x.size();
//    const uint n = x[0].size();
//    std::vector<vec2f> musigma(n, vec2f(0,0));

//    int i = 0;
//    for (auto it1 = x.begin(); it1 != x.end(); ++it1, ++i) {
//        float ri = 1.f/float(i+1);
//        std::transform(musigma.begin(), musigma.end(), (*it1).begin(), musigma.begin(),
//                       [ri](const vec2f& ms, float xij){
//            vec2f curms = ms;
//            float delta = xij - curms.x;
//            curms.x += delta*ri;
//            curms.y += delta*(xij - curms.x);
//            return curms;
//        });
//    }

//    float rm = 1.f/float(m-1);
//    std::transform(musigma.begin(), musigma.end(), musigma.begin(),
//        [rm](const vec2f& ms){ float y = std::sqrt(ms.y/rm); return vec2f(ms.x, y); });

//    return musigma;
//}

//std::vector<float> vec_linear_regression(
//        const std::vector< std::vector<float> > &x
//        , const std::vector<float> &y
//        , float a = 0.01f               // 0.001 0.01 0.1 1 10 ...
//        , uint max_steps = 1500         // 100 1,000 10,000
//        , float presicion = 0.00001f    // 0.1 0.01 0.001 0.0001
//        )
//{
//    const uint m = x.size();
//    const uint n = x[0].size();
//    std::vector<float> th(n, 0.f);

//    const float adm = a/(float)m;
//    uint step = 0;
//    float curn_cost = 10000.f;
//    float prev_cost = curn_cost + presicion + 1.f;

//    while (prev_cost - curn_cost > presicion) {
//        prev_cost = curn_cost;

//        std::vector<float> isum(m, 0.f);
//        std::vector<float> sum(n, 0.f);

//        std::transform(x.begin(), x.end(), y.begin(), isum.begin(),
//                       [th](const std::vector<float>& xi, float yi){ return vec_innprod(th,xi) - yi; });

//        curn_cost = std::accumulate(isum.begin(), isum.end(), 0.f,
//                                    [](float sum, float s){ return sum + sq(s); });

//        std::transform(x.begin(), x.end(), sum.begin(),
//                       [isum](const std::vector<float>& xi){ return vec_innprod(isum, xi); });

//        std::transform(th.begin(), th.end(), sum.begin(), th.begin(),
//                       [adm](float thj, float sumj){ return thj - adm*sumj; });

//        curn_cost *= 0.5f/(float)m;

//        if (curn_cost > prev_cost) {
//            std::cout << "J is increasing: a is too large" << std::endl;
//            break;
//        }

//        if (++step > max_steps) {
//            std::cout << "slow converge: a is too small" << std::endl;
//            break;
//        }
//    }

//    return th;
//}

//}
