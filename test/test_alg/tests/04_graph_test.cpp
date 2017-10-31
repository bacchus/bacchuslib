#include "setting.h"
#include "math/graph.h"
#include "utils/logger.h"

#if TEST_04_GRAPHT

using namespace bacchus;

TEST(graph_test, BFS_DFS) {
    Graph g(8);
    g.insert(0, {1});
    g.insert(1, {0,2});
    g.insert(2, {1,3});
    g.insert(3, {2,4,5});
    g.insert(4, {3,5,6});
    g.insert(5, {3,4,6,7});
    g.insert(6, {4,5,7});
    g.insert(7, {5,6});

    std::vector<int> dist(g.size(), BCC_M_INT_MAX);
    std::vector<int> prnt(g.size(), nil);
    std::vector<int> colr(g.size(), 0);//white
    bfs(g, 2, dist, prnt, colr);

    PRINT(dist);
    PRINT(prnt);

    print_path(prnt, 2, 7);
    LNEND();

    dfs(g);
}

//TEST(graph_test, TopologicSort) {
//    print_msg("---------- graph test topologic sort");
//    enum Cloth { unpants=0, socks, pants, shoes, shirt, belt, tie, watch, jacket};

//    Graph g;
//    g.insert(unpants, {pants, shoes});
//    g.insert(socks, {shoes});
//    g.insert(pants, {shoes, belt});
//    g.insert(shirt, {tie, belt});
//    g.insert(tie, {jacket});
//    g.insert(belt, {jacket});
//    g.insert(shoes, {});
//    g.insert(watch, {});
//    g.insert(jacket, {});

//    dfs(g);
//}

TEST(graph_test, SCC) {
    Graph g(8);
    g.insert(0, {1});
    g.insert(1, {4,5,2});
    g.insert(2, {3,6});
    g.insert(3, {2,7});
    g.insert(4, {0,5});
    g.insert(5, {6});
    g.insert(6, {5,7});
    g.insert(7, {7});

    strongly_connectes_components(g);
}

//====================================================================
TEST(graph_test, MST) {
    Graph g(9);
    g.insertw(0, {{1,4},{7,8}});
    g.insertw(1, {{2,8},{7,11},{0,4}});
    g.insertw(2, {{3,7},{5,4},{8,2},{1,8}});
    g.insertw(3, {{4,9},{5,14},{2,7}});
    g.insertw(4, {{5,10},{3,9}});
    g.insertw(5, {{6,2},{2,4},{3,14},{4,10}});
    g.insertw(6, {{7,1},{8,6},{5,2}});
    g.insertw(7, {{8,7},{0,8},{1,11},{6,1}});
    g.insertw(8, {{2,2},{6,6},{7,7}});

    {
        auto q = mst_kruskal(g);
        int sum = 0;
        std::for_each(q.begin(), q.end(), [&sum](decltype(*q.begin()) x){sum+=x.z;});
        std::cout << "sum: " << sum;
        PRINT(q);
        EXPECT_EQ(37, sum);
    }

    {
        int sum = 0;
        auto prim = mst_prim(g);
        std::for_each(prim.begin(), prim.end(), [&sum](decltype(*prim.begin()) x){sum+=x.z;});
        std::cout << "sum: " << sum;
        PRINT(prim);
        EXPECT_EQ(37, sum);
    }

    {
        int sum = 0;
        std::vector<vec3i> fib = mst_prim_fib(g,0);
        for (int i = 1; i < (int)fib.size(); ++i) {
            sum += g.get(fib[i].x, i);
        }
        std::cout << "sum: " << sum;
        PRINT(fib);
        EXPECT_EQ(37, sum);
    }
}

//====================================================================
TEST(graph_test, BellmanFord) {
    Graph g(5);
    g.insertw(0, {{1,6},{4,7}});
    g.insertw(1, {{2,5},{3,-4},{4,8}});
    g.insertw(2, {{1,-2}});
    g.insertw(3, {{0,2},{2,7}});
    g.insertw(4, {{2,-3},{3,9}});

    std::vector<vec3i> prnt_vert_dist;
    bool res = path_bellman_ford(prnt_vert_dist, g,0);

    EXPECT_EQ(true, res);
    int sum = 0; for (const vec3i& v: prnt_vert_dist) sum += v.z;
    EXPECT_EQ(11, sum);

    print_path(prnt_vert_dist, 0, 3);
    std::cout << std::endl;

    PRINT(prnt_vert_dist);
}

TEST(graph_test, Dag) {
    Graph g(6);
    g.insertw(0, {{1,5},{2,3}});
    g.insertw(1, {{2,2},{3,6}});
    g.insertw(2, {{3,7},{4,4},{5,2}});
    g.insertw(3, {{4,-1},{5,1}});
    g.insertw(4, {{5,-2}});
    g.insertw(5, {});

    std::vector<vec3i> prnt_vert_dist;
    path_dag(prnt_vert_dist, g,1);

    EXPECT_LT(100, prnt_vert_dist[0].z);
    prnt_vert_dist[0].z = 0;
    int sum = 0; for (const vec3i& v: prnt_vert_dist) sum += v.z;
    EXPECT_EQ(16, sum);

    PRINT(prnt_vert_dist);
}

//TEST(graph_test, Dijkstra) {
//    Graph g;
//    g.insertw(0, {{1,16},{2,13}});
//    g.insertw(1, {{2,10},{3,12}});
//    g.insertw(2, {{4,14}});
//    g.insertw(3, {{2,9},{5,20}});
//    g.insertw(4, {{5,4}});
//    g.insertw(5, {});

//    path_dijkstra(g,0);
//    path_dijkstra_mm(g,0);
//}

TEST(graph_test, Dijkstra) {
    Graph g(5);
    g.insertw(0, {{1,10},{4,5}});
    g.insertw(1, {{2,1},{4,2}});
    g.insertw(2, {{3,4}});
    g.insertw(3, {{0,7},{2,6}});
    g.insertw(4, {{1,3},{2,9},{3,2}});

    std::vector<vec3i> prnt_vert_dist;
    path_dijkstra_fib(prnt_vert_dist, g,0);
    PRINT(prnt_vert_dist);
    int sum = 0; for (const vec3i& v: prnt_vert_dist) sum += v.z;
    EXPECT_EQ(29, sum);

    path_dijkstra(g,0);
    path_dijkstra_mm(g,0);
}

//====================================================================
TEST(graph_test, AllShortestPath) {
    GraphM g({
                 {0,3,8,inf,-4},
                 {inf,0,inf,1,7},
                 {inf,4,0,inf,inf},
                 {2,inf,-5,0,inf},
                 {inf,inf,inf,6,0}
             });
    GraphM res = get_allpairs_path(g);
    std::cout << "all pairs\n" << res << std::endl;

    res = transitive_closure(g);
    std::cout << "transitive closure(\n" << res << std::endl;

    res = floyd_warshall(g);
    std::cout << "floyd_warshall\n" << res << std::endl;

    res = floyd_warshall_prnt(g);
    std::cout << "floyd_warshall prnt\n" << res << std::endl;

    std::cout << "path 2 -> 0" << std::endl;
    print_path(res, 2, 0);
    LNEND();
}

TEST(graph_test, Johnson) {
    Graph g(5);
    g.insertw(0, {{1,3},{2,8},{4,-4}});
    g.insertw(1, {{3,1},{4,7}});
    g.insertw(2, {{1,4}});
    g.insertw(3, {{0,2},{2,-5}});
    g.insertw(4, {{3,6}});

    GraphM res;
    bool success = johnson(res, g);
    std::cout << res << std::endl;

    EXPECT_EQ(true, success);
}

//====================================================================
TEST(graph_test, FordFlukerson) {
    Graph g(6);
    g.insertw(0, {{1,16},{2,13}});
    g.insertw(1, {{3,12}});
    g.insertw(2, {{1,4},{4,14}});
    g.insertw(3, {{2,9},{5,20}});
    g.insertw(4, {{3,7},{5,4}});
    g.insertw(5, {});

    ford_fulkerson(g, 0, 5);
}

TEST(graph_test, FordFlukersonDegenerate) {
    Graph g(4);
    g.insertw(0, {{1,1000},{2,1000}});
    g.insertw(1, {{2,1},{3,1000}});
    g.insertw(2, {{3,1000}});
    g.insertw(3, {});

    ford_fulkerson(g, 0, 3);
}

//TEST(graph_test, PushRelabel) {
//    Graph g;
//    g.insertw(0, {{1,16},{2,13}});
//    g.insertw(1, {{3,12}});
//    g.insertw(2, {{1,4},{4,14}});
//    g.insertw(3, {{2,9},{5,20}});
//    g.insertw(4, {{3,7},{5,4}});
//    g.insertw(5, {});

//    generic_push_relabel(g, 0, 5);
//}

//====================================================================
TEST(graph_test, Articulate) {
    {
        Graph g(5);
        g.insert(0, {1,2,3});
        g.insert(1, {0,2});
        g.insert(2, {0,1});
        g.insert(3, {0,4});
        g.insert(4, {3});

        articulation_points(g); // 0 3
    }

    {
        Graph g(4);
        g.insert(0, {1});
        g.insert(1, {0,2});
        g.insert(2, {1,3});
        g.insert(3, {2});

        articulation_points(g); // 1 2
    }

    {
        Graph g(7);
        g.insert(0, {1,2});
        g.insert(1, {0,2,3,4,6});
        g.insert(2, {0,1});
        g.insert(3, {1,5});
        g.insert(4, {1,5});
        g.insert(5, {3,4});
        g.insert(6, {1});

        articulation_points(g); // 1
    }
}
//====================================================================
TEST(graph_test, Bridges) {
    {
        Graph g(5);
        g.insert(0, {1,2,3});
        g.insert(1, {0,2});
        g.insert(2, {0,1});
        g.insert(3, {0,4});
        g.insert(4, {3});

        bridge_edges(g); // 3-4 0-3
    }

    {
        Graph g(4);
        g.insert(0, {1});
        g.insert(1, {0,2});
        g.insert(2, {1,3});
        g.insert(3, {2});

        bridge_edges(g); // 2-3 1-2 0-1
    }

    {
        Graph g(7);
        g.insert(0, {1,2});
        g.insert(1, {0,2,3,4,6});
        g.insert(2, {0,1});
        g.insert(3, {1,5});
        g.insert(4, {1,5});
        g.insert(5, {3,4});
        g.insert(6, {1});

        bridge_edges(g); // 1-6
    }
}

#endif
