#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <deque>

#include "mdefs.h"
#include "vec3.h"

#define nil -1
#define inf BCC_M_INT_MAX

namespace bacchus {

//====================================================================
/// adjacency-list representation
/// V:  std::set
/// Ei: std::map<u, weight>
/// E:  std::map<v, Ei>
class Graph {
public:
    typedef std::set<int>               vlist_type;
    typedef std::map<int,int>           adj_type;
    typedef std::map<int, adj_type>     elist_type;

    Graph() = default;
    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    Graph(const vlist_type& v): m_vlist(v) {}

    uint vsize() const {
        return m_vlist.size();
    }

    const adj_type& adj(int v) const {
        assert(m_data.count(v));
        return m_data.at(v);
    }

    const vlist_type& vlist() const {
        return m_vlist;
    }

    void insertw(int v, const adj_type& adj_list) {
        assert(m_data.count(v)==0);
        m_vlist.insert(v);
        m_data[v] = adj_list;
    }

    void insert(int v, const vlist_type& adj_list) {
        assert(m_data.count(v)==0);
        m_vlist.insert(v);
        for (auto u: adj_list) m_data[v][u] = 1;
    }

    /// adds v to V and w(v,u)
    void add(int v, int u, int w=1) {
        addv(v);
        adde(v,u,w);
    }

    /// adds v to V
    void addv(int v) {
        m_vlist.insert(v);
    }

    /// adds w(v,u)
    void adde(int v, int u, int w=1) {
        m_data[v][u] = w;
    }

    void set(int v, int u, int w=1) {
        assert(m_data.count(v)!=0);
        assert(m_data[v].count(u)!=0);
        m_data[v][u] = w;
    }

    int get(int v, int u) const {
        assert(m_data.count(v)!=0);
        assert(m_data.at(v).count(u)!=0);
        return m_data.at(v).at(u);
    }

    void erase(int v) {
        m_vlist.erase(v);
        m_data.erase(v);
    }

private:
    elist_type m_data;// v -> u,w
    vlist_type m_vlist;// v-s
};

inline std::ostream& operator <<(std::ostream& ostr, const Graph& mat) {
    for (auto v: mat.vlist()) {
        ostr << v << ": ";
        for (auto u: mat.adj(v)) {
            ostr << u.first << "(" << u.second << ") ";
        }
        ostr<<std::endl;
    }
    ostr<<std::endl;
    return ostr;
}

//====================================================================
/// general algs for any graphs: BFS, DFS
void print_path(const std::vector<int>& parnt, int s, int v);
void print_path(const std::vector<vec3i> &prnt_vert_dist, int s, int v);
int dist_path(const Graph& g, const std::vector<vec3i> &prnt_vert_dist, int s, int v);
void bfs(const Graph& g, int s, std::vector<int>& dist, std::vector<int>& prnt, std::vector<int>& colr);// O(V + E)
void dfs(const Graph& g);// O(V + E)

/// use of BFS and DFS
void find_undirected_connected_components(const Graph& g); // use BFS
std::deque<int> topological_sort(const Graph& g); // use DFS

/// articulation_points, bridges
void articulation_points(const Graph& g);
void bridge_edges(const Graph& g);

/// oriented graph
Graph transpose(const Graph& g);
void strongly_connectes_components(const Graph& g);

/// wheighted nonoriented minimal spanning tree
std::vector<vec3i> mst_kruskal(const Graph& g);
std::vector<vec3i> mst_prim(const Graph& g);  /// TODO: deprecated, use _fib
std::vector<vec3i> mst_prim_fib(const Graph& g, int r);

/* wheighted oriented graph shortest path from one vertex
 * if w < 0, negative cycles    - Bellman-Ford  - O(V*E)
 * if w < 0, no negative cycles - Dag           - O(E + V)
 * if w > 0,                    - Dijkstra
 *      nondecreasing priority queue as
 *          linear array                        - O(V^2 + E)
 *          binary nondecreasing pyramid        - O(E*lg(V))
 *          fibonachi pyramid                   - O(V*lg(V) + E)
 */
bool path_bellman_ford(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s);
void path_dag(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s);
void path_dijkstra_fib(std::vector<vec3i>& prnt_vert_dist, const Graph& g, int s);
void path_dijkstra(const Graph& g, int s);      /// TODO: deprecated, use _fib
void path_dijkstra_mm(const Graph &g, int s);   /// TODO: deprecated, use _fib

//====================================================================
/// matrix representation: vector<vector<int>>
class GraphM {
public:
    typedef std::vector< std::vector<int> > adjmat_type;
    GraphM() {}
    GraphM(uint n, int x=nil): m_data(n, std::vector<int>(n,x)) {}
    GraphM(const adjmat_type& data): m_data(data) {}
    GraphM(const GraphM&) = default;
    GraphM& operator=(const GraphM&) = default;
    GraphM(GraphM&&) = default;
    GraphM& operator=(GraphM&&) = default;
    uint vsize() const { return m_data.size(); }
    int operator()(uint v, uint u) const { return m_data[v][u]; }
    int& operator()(uint v, uint u) { return m_data[v][u]; }
private:
    adjmat_type m_data;
};

inline std::ostream& operator <<(std::ostream& ostr, const GraphM& mat) {
    for(uint i=0; i < mat.vsize(); ++i) {
        for (uint j = 0; j < mat.vsize(); ++j) {
            if (mat(i,j)<inf)
                ostr << std::setw(2) << mat(i,j) << " ";
            else
                ostr << std::setw(2) << "#" << " ";
        }
        ostr<<std::endl;
    }
    ostr<<std::endl;
    return ostr;
}

//====================================================================
/// find shortest paths for all V, graph represented by matrix
void print_path(const GraphM& d, int i, int j);
GraphM get_allpairs_path(const GraphM& w);      // O(V^3*lg(V))
GraphM floyd_warshall(const GraphM& w);         // O(V^3)
GraphM floyd_warshall_prnt(const GraphM& w);    // O(V^3)
GraphM transitive_closure(const GraphM& w);     // O(V^3)
/// use Dijkstra |V| times, for que as fib-heap
bool johnson(GraphM& res, const Graph& g);      // O(V^2*lg(V) + V*E)
//====================================================================
/// flow network
void ford_fulkerson(const Graph& g, int s, int t);

} // namespace bacchus
