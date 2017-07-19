#include "graph.h"

#include <vector>
#include <list>
#include <algorithm>

#include "mdefs.h"
#include "fib_heap.h"
#include "utils/logger.h"
#include "utils/print.h"

namespace bacchus {

//const std::vector<int> Graph::m_empty_list;

enum VisitColor {
    WHIT = 0,// novisited
    GRAY = 1,// discovered
    BLAC = 2// processed
};

//const int nil = -1;// points to no

//====================================================================
void print_path(const std::vector<int> &prnt, int s, int v) {
    if (v == s) {
        std::cout << s << " ";
    } else if (prnt[v] == nil) {
        std::cout << "no way" << std::endl;
    } else {
        print_path(prnt, s, prnt[v]);
        std::cout << v << " ";
    }
}

void print_path(const std::vector<vec3i> &prnt_vert_dist, int s, int v) {
    if (v == s) {
        std::cout << s << " ";
    } else if (prnt_vert_dist[v].x == nil) {
        std::cout << "no way" << std::endl;
    } else {
        print_path(prnt_vert_dist, s, prnt_vert_dist[v].x);
        std::cout << v << " ";
    }
}
//====================================================================
void bfs(const Graph& g, int s
         , std::vector<int>& dist
         , std::vector<int>& prnt
         , std::vector<int>& colr
         )
{
//    std::vector<int> colr(g.vsize(), white);
//    std::vector<int> dist(g.vsize(), BCC_M_INT_MAX);
//    std::vector<int> prnt(g.vsize(), nil);

//    colr.clear();
//    dist.clear();
//    prnt.clear();

//    colr.resize(g.vsize(), white);
//    dist.resize(g.vsize(), BCC_M_INT_MAX);
//    prnt.resize(g.vsize(), -1);

    colr[s] = GRAY;
    dist[s] = 0;
    prnt[s] = nil;

    std::deque<int> que{s};
    while (!que.empty()) {
        int u = que.front();
        que.pop_front();
        for (auto v: g.adj(u)) {
            if (colr[v.first] == WHIT) {
                colr[v.first] = GRAY;
                dist[v.first] = dist[u] + 1;
                prnt[v.first] = u;
                que.push_back(v.first);
            }
        }
        colr[u] = BLAC;
    }
}

//====================================================================
void find_undirected_connected_components(const Graph& g) {
    std::vector<int> dist(g.size(), BCC_M_INT_MAX);
    std::vector<int> prnt(g.size(), nil);
    std::vector<int> colr(g.size(), WHIT);

    std::cout << "cc: ";
    for (auto v: g.vlist()) {
        if (colr[v]==WHIT) {
            std::cout << v << " ";
            bfs(g,v, dist,prnt,colr);
        }
    }
    std::cout << std::endl;
}

//====================================================================
void dfs_visit(const Graph& g, int u, int& t
               , std::vector<int>& colr
               , std::vector<int>& prnt
               , std::vector<int>& dist
               , std::vector<int>& finl
//               , std::deque<int>& sortd
               )
{
    //std::cout<<"(";
    colr[u] = GRAY;
    dist[u] = ++t;
    for (auto v: g.adj(u)) {
        if (colr[v.first]==WHIT) {
            prnt[v.first] = u;
            dfs_visit(g,v.first,t, colr, prnt, dist, finl/*, sortd*/);
        }
    }
    colr[u] = BLAC;
    finl[u] = ++t;
    //sortd.push_front(u);
    //std::cout<<")";
}

void dfs(const Graph& g) {
    std::vector<int> colr(g.size(), WHIT);
    std::vector<int> prnt(g.size(), nil);
    std::vector<int> dist(g.size(), 0);
    std::vector<int> finl(g.size(), 0);
//    std::deque<int> sortd;
    int t = 0;
    for (auto u: g.vlist()) {
        if (colr[u]==WHIT) {
            dfs_visit(g,u,t, colr, prnt, dist, finl/*, sortd*/);
        }
    }
//    std::string names[] = {"unpants", "socks", "pants", "shoes", "shirt", "belt", "tie", "watch", "jacket"};
//    for (auto x: sortd) {
//        std::cout << names[x] << " ";
//    }
//    print_msg();

    PRINT(colr);
    PRINT(prnt);
    PRINT(dist);
    PRINT(finl);
}

//====================================================================
void sort_dfs_visit(const Graph& g, int u, int& t
               , std::vector<int>& colr
               , std::vector<int>& prnt
               , std::vector<int>& dist
               , std::vector<int>& finl
               , std::deque<int>& sortd
               )
{
    colr[u] = GRAY;
    ++t;
    dist[u] = t;
    for (auto v: g.adj(u)) {
        if (colr[v.first]==WHIT) {
            prnt[v.first] = u;
            sort_dfs_visit(g,v.first,t, colr, prnt, dist, finl, sortd);
        }
    }
    colr[u] = BLAC;
    finl[u] = ++t;
    sortd.push_front(u);
}

std::deque<int> topological_sort(const Graph& g) {
    std::vector<int> colr(g.size(), WHIT);
    std::vector<int> prnt(g.size(), nil);
    std::vector<int> dist(g.size(), 0);
    std::vector<int> finl(g.size(), 0);
    std::deque<int> sortd;
    int t = 0;
    for (auto u: g.vlist()) {
        if (colr[u]==WHIT) {
            sort_dfs_visit(g,u,t, colr, prnt, dist, finl, sortd);
        }
    }
    return sortd;
}

//====================================================================
Graph transpose(const Graph& g) {
    Graph tr;
    for (auto u: g.vlist()) {
        for (auto v: g.adj(u)) {
            tr.add(v.first,u);
        }
    }
    return tr;
}

void strongly_connectes_components(const Graph& g) {
    std::deque<int> gsortd = topological_sort(g);
    for (auto x: gsortd)
        std::cout << x << " ";
    std::cout << std::endl;
    Graph tr = transpose(g);

    std::vector<int> colr(g.size(), WHIT);
    std::vector<int> prnt(g.size(), nil);
    std::vector<int> dist(g.size(), 0);
    std::vector<int> finl(g.size(), 0);
    int t = 0;
    for (auto u: gsortd) {
        if (colr[u]==WHIT) {
            std::deque<int> trsortd;
            sort_dfs_visit(tr,u,t, colr, prnt, dist, finl, trsortd);
            for (auto x: trsortd) std::cout << x << " ";
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

//====================================================================
std::vector<vec3i> mst_kruskal(const Graph& g) {
    // sort edges by weight
    std::vector<vec3i> edges;
    for (auto v: g.vlist()) {
        for (auto u: g.adj(v)) {
            edges.push_back(vec3i(v,u.first,u.second));
        }
    }
    std::sort(edges.begin(), edges.end(), [](const vec3i&a, const vec3i&b){ return a.z<b.z; });

    // make sets of verts - for v in vlist: make-set(v)
    int i = 0;
    std::vector<int> sets(g.size());
    std::generate(sets.begin(), sets.end(), [&i](){return i++;});

    // visit edges in weight order
    std::vector<vec3i> res;
    for (const vec3i& e: edges) {
        int sx = sets[e.x];
        int sy = sets[e.y];
        // find-set(x) != find-set(y)
        if (sx!=sy) {
            // A U A {(x,y)}
            res.push_back(e);

            // union(u,v)
            for (int& j: sets) {
                if (j==sy)
                    j = sx;
            }
        }
    }
    return res;
}

//====================================================================
/// mst_prim(Graph g, Node r)
std::vector<vec3i> mst_prim_fib(const Graph& g, int r) {
    const int n = g.size();

    /// for u: g.vlist
    ///     u.key = inf
    ///     u.p = nil
    // i-vetr, vec3i(prnt, vert, key)
    std::vector<vec3i> prnt_vert_dist(n, vec3i(-1, -1, BCC_M_INT_MAX));

    /// r.key = 0
    prnt_vert_dist[r].z = 0;

    /// Q = g.vlist
    typedef FibHeap<int,vec2i> Que;
    Que que;
    std::vector<Que::Node*> nodes(n, nullptr);
    for (int i = 0; i < n; ++i) {
        prnt_vert_dist[i].y = i;
        Que::Node* node = new Que::Node(prnt_vert_dist[i].z, prnt_vert_dist[i].xy());
        que.insert(node);
        nodes[i] = node;
    }

    /// while Q != 0
    while (!que.empty()) {
        /// u = extract_min(Q)
        Que::Node* node = que.extract_min();
        vec2i u = node->value;
        delete node;
        nodes[u.y] = nullptr;

        /// for v: g.adj(u)
        for (auto vw: g.adj(u.y)) {

            int v = vw.first;    // v
            int w = vw.second;   // W(u,v)

            /// if v in Q && W(u,v) < v.key
            if (nodes[v] != nullptr && w < prnt_vert_dist[v].z) {
                /// v.p = u
                prnt_vert_dist[v].x = u.y;

                /// v.key = W(u,v)
                prnt_vert_dist[v].z = w;

                /// decrease_key(Q,v,W(u,v))
                que.decrease_key(nodes[v], w);
            }
        }
    }

    return prnt_vert_dist;
}

// next is stupid implementation
std::vector<vec3i> mst_prim(const Graph& g) {
    std::set<int> que(g.vlist().begin(), g.vlist().end());
    std::vector<vec3i> res;
    que.erase(0);
    while (!que.empty()) {
        vec3i minv(BCC_M_INT_MAX);
        for (auto v: que) {
            for (auto u: g.adj(v)) {
                if (!que.count(u.first) && u.second < minv.z) {
                    minv.x = v;
                    minv.y = u.first;
                    minv.z = u.second;
                }
            }
        }
        que.erase(minv.x);
        res.push_back(minv);
    }
    return res;
}

//====================================================================
bool relax(int u, int v, int w, std::vector<vec3i>& prnt_vert_dist) {
    if (prnt_vert_dist[v].z > prnt_vert_dist[u].z + w) {
        prnt_vert_dist[v].z = prnt_vert_dist[u].z + w;
        prnt_vert_dist[v].x = u;
        return true;
    }
    return false;
}

bool path_bellman_ford(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s) {
    // i-vetr, vec3i(prnt, vert, key)
    const int n = g.size();
    prnt_vert_dist = std::vector<vec3i>(n, vec3i(-1, -1, BCC_M_INT_MAX));
    for (int i = 0; i < n; ++i)
        prnt_vert_dist[i].y = i;
    prnt_vert_dist[s].z = 0;

    for (int i = 0; i < g.size()-1; ++i) {
        for (auto u: g.vlist()) {
            for (auto v: g.adj(u)) {
                relax(u,v.first,v.second, prnt_vert_dist);
            }
        }
    }
    for (auto u: g.vlist()) {
        for (auto v: g.adj(u)) {
            if (prnt_vert_dist[v.first].z > prnt_vert_dist[u].z + v.second) {
                return false;
            }
        }
    }
    return true;
}

void path_dag(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s) {
    // i-vetr, vec3i(prnt, vert, key)
    const int n = g.size();
    prnt_vert_dist = std::vector<vec3i>(n, vec3i(-1, -1, BCC_M_INT_MAX));
    for (int i = 0; i < n; ++i)
        prnt_vert_dist[i].y = i;
    prnt_vert_dist[s].z = 0;

    auto sortd = topological_sort(g);
    for (auto u: sortd) {
        for (auto v: g.adj(u)) {
            relax(u, v.first, v.second, prnt_vert_dist);
        }
    }
}

//====================================================================
/// path_dijkstra_fib(Graph g, Node s)
void path_dijkstra_fib(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s) {
    // i-vetr, vec3i(prnt, vert, key)
    const int n = g.size();
    prnt_vert_dist = std::vector<vec3i>(n, vec3i(-1, -1, BCC_M_INT_MAX));
    for (int i = 0; i < n; ++i)
        prnt_vert_dist[i].y = i;
    prnt_vert_dist[s].z = 0;

    /// Q = g.vlist
    typedef FibHeap<int,vec2i> Que;
    Que que;
    std::vector<Que::Node*> nodes(n, nullptr);
    for (int i = 0; i < n; ++i) {
        prnt_vert_dist[i].y = i;
        Que::Node* node = new Que::Node(prnt_vert_dist[i].z, prnt_vert_dist[i].xy());
        que.insert(node);
        nodes[i] = node;
    }

    /// while Q != 0
    while (!que.empty()) {
        /// u = extract_min(Q)
        Que::Node* node = que.extract_min();
        vec2i u = node->value;
        delete node;
        nodes[u.y] = nullptr;

        /// for v: g.adj(u)
        for (auto vw: g.adj(u.y)) {

            int v = vw.first;    // v
            int w = vw.second;   // W(u,v)

            if (relax(u.y,v,w, prnt_vert_dist)) {
                que.decrease_key(nodes[v], prnt_vert_dist[v].z);
            }
        }
    }
}

// next is stupid implementation: using std::list and std::sort
void path_dijkstra(const Graph &g, int s) {
    std::set<int> vlist = g.vlist();
    vlist.erase(s);

    std::vector<int> prnt(g.size(), nil);
    std::vector<int> dist(g.size(), BCC_M_INT_MAX);
    dist[s] = 0;

    std::list<vec3i> edges;

    for (auto u: g.adj(s)) {
        edges.push_back(vec3i(s,u.first, u.second));
    }
    edges.sort([](const vec3i&a, const vec3i&b){ return a.z<b.z; });

    while (!vlist.empty()) {
        vec3i vuw = edges.front();
        dist[vuw.y] = dist[vuw.x] + g.get(vuw.x, vuw.y);
        prnt[vuw.y] = vuw.x;
        edges.remove(vuw);
        for (auto u: g.adj(vuw.y)) {
            edges.push_back(vec3i(vuw.y, u.first, u.second));
        }
        edges.sort([](const vec3i&a, const vec3i&b){ return a.z<b.z; });
        vlist.erase(vuw.y);
    }

    PRINT(dist);
    PRINT(prnt);
}

// next is stupid implementation: using std::multimap
void path_dijkstra_mm(const Graph &g, int s) {
    std::set<int> vlist = g.vlist();
    vlist.erase(s);

    std::vector<int> prnt(g.size(), nil);
    std::vector<int> dist(g.size(), BCC_M_INT_MAX);
    dist[s] = 0;

    std::multimap<int, vec2i> edges;

    for (auto u: g.adj(s)) {
        edges.emplace(u.second, vec2i(s,u.first));
    }

    while (!vlist.empty()) {
        auto e = edges.begin();
        int v = e->second.x;
        int u = e->second.y;
        edges.erase(e);
        if (!vlist.count(u)) {
            std::cout << "continue: " << v << "->" << u << std::endl;
            continue;
        }
        vlist.erase(u);
        dist[u] = dist[v] + g.get(v, u);
        prnt[u] = v;
        for (auto nu: g.adj(u)) {
            edges.emplace(nu.second, vec2i(u, nu.first));
        }
    }
    PRINT(dist);
    PRINT(prnt);
}

//====================================================================
void print_path(const GraphM &prnt, int i, int j) {
    if (i==j) {
        std::cout << i << " ";
    } else if (prnt(i,j) == nil) {
        std::cout << "no way" << std::endl;
    } else {
        print_path(prnt, i, prnt(i,j));
        std::cout << j << " ";
    }
}

GraphM extend_shortest_path(const GraphM &l, const GraphM &w) {
    uint n = l.vsize();
    GraphM l2(n);
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            l2(i,j) = BCC_M_INT_MAX;
            for (uint k = 0; k < n; ++k) {
                l2(i,j) = std::min(l2(i,j), l(i,k) + w(k,j));
            }
        }
    }
    return l2;
}

GraphM get_allpairs_path(const GraphM &w) {
    uint n = w.vsize();
    GraphM dist(w);
    uint m = 1;
    while (m < n-1) {
        dist = extend_shortest_path(dist,dist);
        m *= 2;
    }
    // O(n^4)
//    for (uint m = 2; m < n; ++m) {
//        l = extend_shortest_path(l,w);
//    }
    return dist;
}

GraphM floyd_warshall(const GraphM &w) {
    uint n = w.vsize();
    GraphM dist(w);
    for (uint k = 0; k < n; ++k) {
        for (uint i = 0; i < n; ++i) {
            for (uint j = 0; j < n; ++j) {
                dist(i,j) = std::min(dist(i,j), dist(i,k) + dist(k,j));
            }
        }
    }
    return dist;
}

GraphM floyd_warshall_prnt(const GraphM &w) {
    uint n = w.vsize();
    GraphM dist(w);
    GraphM prnt(n);
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            if (i!=j && w(i,j)<inf) prnt(i,j) = i;
            else                    prnt(i,j) = nil;
        }
    }

    for (uint k = 0; k < n; ++k) {
        for (uint i = 0; i < n; ++i) {
            for (uint j = 0; j < n; ++j) {
                //dist(i,j) = std::min(dist(i,j), dist(i,k) + dist(k,j));
                int dij = dist(i,j);
                int dikj = dist(i,k) + dist(k,j);
                if (dikj < dij) {
                    dist(i,j) = dikj;
                    prnt(i,j) = prnt(k,j);
                } else {
                    dist(i,j) = dij;
                }
            }
        }
    }
    return prnt;
}

GraphM transitive_closure(const GraphM &w) {
    uint n = w.vsize();
    GraphM trns(n);
    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < n; ++j) {
            if (i==j || w(i,j)<inf) trns(i,j) = 1;
            else                    trns(i,j) = 0;
        }
    }
    // min -> ||
    //  +  -> &&
    for (uint k = 0; k < n; ++k) {
        for (uint i = 0; i < n; ++i) {
            for (uint j = 0; j < n; ++j) {
                trns(i,j) = trns(i,j) || (trns(i,k) && trns(k,j));
            }
        }
    }
    return trns;
}

void ford_fulkerson(const Graph &g, int s, int t) {
    Graph f;
    for (auto v: g.vlist()) {
        for (auto u: g.adj(v)) {
            f.add(v,u.first,0);
            f.add(u.first,v,0);
        }
    }

    bool found = true;
    int n = 0;
    while (found) {
        found = false;
        std::vector<int> colr(g.size(), WHIT); colr[s] = GRAY;
        std::vector<int> prnt(g.size(), nil); prnt[s] = nil;
        std::deque<int> que{s};
        while (!que.empty() && !found) {
            int u = que.front();
            que.pop_front();
            for (auto v: g.adj(u)) {
                int fuv = f.get(u,v.first);
                int guv = g.get(u,v.first);
                if ((colr[v.first] == WHIT) && (fuv < guv)) {
                    if (v.first==t) {
                        found = true;
                        prnt[v.first] = u;
                        break;
                    }
                    colr[v.first] = GRAY;
                    prnt[v.first] = u;
                    que.push_back(v.first);
                }
            }
            colr[u] = BLAC;
        }

        if (found) {
            int minc = BCC_M_INT_MAX;
            {
                int v = t;
                while (v != s) {
                    int u = prnt[v];
                    int cfp = g.get(u,v)-f.get(u,v);
                    if (cfp < minc) minc = cfp;
                    v = prnt[v];
                }
            }
            {
                int v = t;
                while (v != s) {
                    int u = prnt[v];
                    int fuv = f.get(u,v);
                    f.set(u,v, fuv + minc);
                    f.set(v,u, -f.get(u,v));
                    v = prnt[v];
                }
            }
        }

        ++n;
    }

    std::cout << "n: " << n << std::endl;
    std::cout << g << std::endl;
    std::cout << f << std::endl;
}



// push flows
bool push(const Graph &g, int u, int v
          , Graph &d
          , Graph &f
          , std::vector<int>& h
          , std::vector<int>& e
          )
{
    if (g.get(u,v)-f.get(u,v) > 0 && h[u]==h[v]+1) {
        d.set(u,v, std::min(e[u], g.get(u,v)-f.get(u,v)));
        f.set(u,v, f.get(u,v) + d.get(u,v));
        f.set(v,u, -f.get(u,v));
        e[u] -= d.get(u,v);
        e[v] += d.get(u,v);
        return true;
    }
    return false;
}

bool relabel(const Graph &g, int u, std::vector<int>& h) {
    int minhv = BCC_M_INT_MAX;
    for (auto v: g.adj(u)) {
        if (h[u] > h[v.first]) return false;
        if (h[v.first] < minhv) minhv = h[v.first];
    }

    h[u] = 1 + minhv;
    return true;
}

void init_preflow(const Graph &g, int s
                  , Graph &f
                  , std::vector<int>& h
                  , std::vector<int>& e
                  )
{
    for (auto u: g.vlist()) {
        for (auto v: g.adj(u)) {
            f.set(u,v.first,0);
            f.set(v.first,u,0);
        }
    }
    h[s] = g.size();
    for (auto u: g.adj(s)) {
        int csu = u.second;
        f.set(s,u.first, csu);
        f.set(u.first,s,-csu);
        e[u.first] = csu;
        e[s] -= csu;
    }
}

void generic_push_relabel(const Graph &g, int s, int t) {
    int n = g.size();
    std::vector<int> h(n,0);
    std::vector<int> e(n,0);
    Graph d;
    Graph f;
    init_preflow(g,s,f,h,e);

    bool exist = true;
    while (exist) {
        exist = false;
        for (auto u: g.vlist()) {
            if (u != s && u!= t && e[u] > 0) {
                exist = relabel(g,u,h);
                for (auto v: g.adj(u)) {
                    exist = exist || push(g,u,v.first,d,f,h,e);
                }
            }
            if (exist) break;
        }
    }

    std::cout << d << std::endl;
    std::cout << f << std::endl;
}

} // namespace bacchus
