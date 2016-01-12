#include "linear_prog.h"

namespace bacchus {

void print_equatoins(
          const std::vector<int> &N
        , const std::vector<int> &B
        , const matxf &A
        , const std::vector<float> &b
        , const std::vector<float> &c
        , float v)
{
    int m = A.m;
    int n = A.n;
    std::cout<<"z = "<<v;
    for (int j = 0; j < n; ++j) {
        std::cout<<" + "<<c[j]<<"*x"<<N[j]+1;
    }
    for (int i = 0; i < m; ++i) {
        std::cout<<std::endl<<"x"<<B[i]+1<<" = "<<b[i];
        for (int j = 0; j < n; ++j) {
            std::cout<<" + "<<-A(i,j)<<"*x"<<N[j]+1;
        }
    }
    std::cout<<std::endl;
}


void print_equatoins(
          const matxf &A
        , const std::vector<float> &b
        , const std::vector<float> &c)
{
    int m = A.m;
    int n = A.n;
    std::cout<<"maximize: ";
    for (int j = 0; j < n; ++j) {
        std::cout<<" + "<<c[j]<<"*x"<<j+1;
    }
    std::cout<<std::endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout<<" + "<<A(i,j)<<"*x"<<j+1;
        }
        std::cout<<" <= "<<b[i]<<std::endl;
    }
    std::cout<<std::endl;
}


int find_min(const std::vector<float> &vec) {
    assert(vec.size() > 0);
    float xmin = vec[0];
    int imin = 0;
    for (uint i = 1; i < vec.size(); ++i) {
        if (vec[i] < xmin) {
            xmin = vec[i];
            imin = i;
        }
    }
    return imin;
}


int positive_index(const std::vector<float> &c) {
    for (uint i = 0; i < c.size(); ++i) {
        if (c[i] > 0)
            return i;
    }
    return -1;
}


int exist_in(int x, const std::vector<int> &vec) {
    int xi = -1;
    for (uint i = 0; i < vec.size(); ++i) {
        if (x == vec[i])
            return i;
    }
    return xi;
}


void pivot(
          std::vector<int> &N
        , std::vector<int> &B
        , matxf &A
        , std::vector<float> &b
        , std::vector<float> &c
        , float &v, int l, int e)
{
    if (dbg) std::cout<<std::endl<<"change base: "<<B[l]+1<<" and nonbase: "<<N[e]+1<<std::endl<<std::endl;
    // compute coefs for new basis x[e]
    int m = A.m;
    int n = A.n;
    //TODO: pivot calls with A(l,e) != 0; in theory
    float div_ale = 1.0f/A(l,e);
    // recompute l-s equasion
    b[l] *= div_ale;
    for (int j = 0; j < n; ++j) {
        if (j != e) {
            A(l,j) *= div_ale;
        }
    }
    A(l,e) = div_ale;
    // recompute other equasions
    for (int i = 0; i < m; ++i) {
        if (i != l) {
            b[i] -= A(i,e)*b[l];
            for (int j = 0; j < n; ++j) {
                if (j != e) {
                    A(i,j) -= A(i,e)*A(l,j);
                }
            }
            A(i,e) *= -div_ale;
        }
    }

    //if (dbg) A.test_print();

    // z func
    v += c[e]*b[l];
    for (int j = 0; j < n; ++j) {
        if (j != e) {
            c[j] -= c[e]*A(l,j);
        }
    }
    c[e] *= -div_ale;

    // swap basis and notbasis in sets B and N
    std::swap(B[l], N[e]);

    if (dbg) print_equatoins(N,B,A,b,c,v);
}


int init_simplex(
          std::vector<int> &N
        , std::vector<int> &B
        , matxf &A
        , std::vector<float> &b
        , std::vector<float> &c
        , float &v)
{
    int m = A.m;
    int n = A.n;
    // find minimal b
    int l = find_min(b);

    // init nonbase set
    for (int j = 0; j < n; ++j) {
        N[j] = j;
    }
    // init base set
    for (int i = 0; i < m; ++i) {
        B[i] = i + n;
    }

    // check base solution: X=0 -> min b >= 0
    if (b[l] >= 0) {
        if (dbg) print_equatoins(N,B,A,b,c,v);
        return 1;
    }

    if (dbg) std::cout<<std::endl<<"searching L aux solution"<<std::endl<<std::endl;

    /// AUX
    // make L aux: x[i] = b[i] - sum(a(i,j)*x[j]) + x0;
    // add new x0 to last x: x[m+n]
    std::vector<int> Naux(N);
    Naux.push_back(n+m);

    // A aux = |A -1|
    matxf Aux(m, n+1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            Aux(i,j) = A(i,j);
        }
        Aux(i,n) = -1.0f;// new element -1
    }
    // new z = -x0
    std::vector<float> caux(n+1, 0);
    caux[n] = -1.0f;
    v = 0;

    if (dbg) print_equatoins(Naux,B,Aux,b,caux,v);

    // change base l with min b and nonbase new element its on n-th position
    pivot(Naux,B,Aux,b,caux,v,l,n);

    // find solution in new system
    int e = positive_index(caux);// find first e in nonbase: c[e]>0; e [0..n]
    //TODO: if e=-1 all indexes negative or 0 - solution if all b[i]>=0 X=0 else no exitst
    while (e > -1) {
        // find base l: A(l,e)>0 and b[l]/A(l,e) is minimal
        int l = -1;
        float dmin = BCC_M_FLOAT_MAX;
        for (int i = 0; i < m; ++i) {
            if (Aux(i,e) > 0) {
                float d = b[i]/Aux(i,e);
                if (d < dmin) {
                    dmin = d;// min b[i]/A(i,e)
                    l = i;   // min val base index
                }
            }
        }
        // if all A(i,e)<=0 system has unlimited solutions
        // else we change base l and nonbase e in pivot()
        if (dmin == BCC_M_FLOAT_MAX) {
            std::cout<<"unlimited"<<std::endl;
            return 0;
        } else {
            pivot(Naux,B,Aux,b,caux,v,l,e);
        }
        // find next c[e]>0
        e = positive_index(caux);
    }

    if (dbg) {
        std::vector<float> res(m+n+1);
        // all nonbase = 0
        for (int j = 0; j < n+1; ++j) {
            res[Naux[j]] = 0;
        }
        // all base = b[i]
        for (int i = 0; i < m; ++i) {
            res[B[i]] = b[i];
        }
        for (uint k = 0; k < res.size(); ++k) {
            std::cout<<res[k]<<" ";
        }
        std::cout<<std::endl;
    }

    // x0 must be 0 to have solution
    //TODO: do x0 necessary be in N
    int jx0 = exist_in(n+m,Naux);
    if (jx0>-1) {
        // rewrite new matrix and N without x0
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < jx0; ++j) {
                A(i,j) = Aux(i,j);
                N[i] = Naux[i];
            }
            for (int j = jx0; j < n; ++j) {
                A(i,j) = Aux(i,j+1);
                N[j] = Naux[j+1];
            }
        }
        for (int j = 0; j < n; ++j) {
            int jb = exist_in(j,B);
            if (jb > -1) {
            }
        }
        // compute v
        v = 0.0f;
        for (int i = 0; i < n; ++i) {
            // replace base elements in z with nonbase
            int jb = exist_in(i,B);
            if (jb > -1) {
                v += c[i]*b[jb];
            }
        }
        // rewrite c of only nonbase elements
        if (dbg) std::cout<<"find c: "<<std::endl;
        for (int j = 0; j < n; ++j) {
            float sum = 0;
            int jn = exist_in(j,N);
            if (jn > -1) {
                sum += c[j];
            }
            for (int i = 0; i < n; ++i) {
                int ib = exist_in(i,B);
                if (ib > -1) {
                    sum -= c[i]*A(ib,j);
                }
            }
            c[j] = sum;
        }
        if (dbg) print_equatoins(N,B,A,b,c,v);
    } else {
        std::cout<<"nonsolvable"<<std::endl;
        return 0;
    }

    return 1;
}


int simplex(
          const matxf &inA
        , const std::vector<float> &inb
        , const std::vector<float> &inc
        , std::vector<float> &outx)
{
    int m =inA.m; // m - equations or base x
    int n = inA.n;// n - nonbase x

    outx.resize(m+n);// m+n - base + nonbase x
    std::vector<int> N(n, 0);// nonbase set
    std::vector<int> B(m, 0);// base set
    matxf A(inA);// same as inA
    std::vector<float> b(inb); // same as b
    std::vector<float> c(inc); // same as c
    float v;

    // check for solve
    int solvable = init_simplex(N,B,A,b,c,v);
    if (solvable==0) {
        return 0;
    }

    int e = positive_index(c);// find first e in nonbase: c[e]>0; e [0..n]
    //TODO: if e=-1 all indexes negative or 0 - solution if all b[i]>=0 X=0 else no exitst
    while (e > -1) {
        // find base l: A(l,e)>0 and b[l]/A(l,e) is minimal
        int l = -1;
        float dmin = BCC_M_FLOAT_MAX;
        for (int i = 0; i < m; ++i) {
            if (A(i,e) > 0) {
                float d = b[i]/A(i,e);
                if (d < dmin) {
                    dmin = d;// min b[i]/A(i,e)
                    l = i;   // min val base index
                }
            }
        }
        // if all A(i,e)<=0 system has unlimited solutions
        // else we change base l and nonbase e in pivot()
        if (dmin == BCC_M_FLOAT_MAX) {
            std::cout<<"unlimited"<<std::endl;
            return 0;
        } else {
            pivot(N,B,A,b,c,v,l,e);
        }
        // find next c[e]>0
        e = positive_index(c);
    }

    // all nonbase = 0
    for (int j = 0; j < n; ++j) {
        outx[N[j]] = 0;
    }
    // all base = b[i]
    for (int i = 0; i < m; ++i) {
        outx[B[i]] = b[i];
    }

    return 1;
}

} // namespace bacchus
