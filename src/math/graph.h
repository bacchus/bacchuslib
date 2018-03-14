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
class Graph {
public:
    using vlist_type = std::vector<int>;        // for list of verts
    using edge_type = std::pair<int,int>;       // vertex, weight
    using adj_type = std::vector<edge_type>;    // list of edges
    using data_type = std::vector<adj_type>;    // vec of vec of pair

    Graph(const Graph&) = default;
    Graph& operator=(const Graph&) = default;
    Graph(Graph&&) = default;
    Graph& operator=(Graph&&) = default;

    Graph(uint num): data(num) {}

    uint size() const { return data.size(); }
    void resize(int n) { data.resize(n); }

    const adj_type& adj(int v) const { return data[v]; }
    adj_type& adj_mut(int v) { return data[v]; }

    void add(int v, int u, int w=1) {
        data[v].emplace_back(u,w);
    }

    void insertw(int v, const adj_type& adj_list) {
        data[v] = adj_list;
    }

    void insert(int v, const vlist_type& adj_list) {
        for (auto u: adj_list)
            data[v].emplace_back(u,1);
    }

    int weight(int v, int u) const {
        for (const edge_type& edge: data[v]) {
            if (edge.first == u)
                return edge.second;
        }
        assert(false); // must never get here
        return 0;
    }

    int& weight_mut(int v, int u) {
        for (edge_type& edge: data[v]) {
            if (edge.first == u)
                return edge.second;
        }
        assert(false); // must never get here
        return data[0][0].second;
    }

private:
    data_type data;
};

inline std::ostream& operator <<(std::ostream& ostr, const Graph& g) {
    for (uint v=0; v<g.size(); ++v) {
        ostr << v << ": ";
        for (auto u: g.adj(v)) {
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
