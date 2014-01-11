#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/graph/graph_utility.hpp>
//#include <boost/graph/graphviz.hpp>

using namespace boost;

int main()
{
    enum { A, B, C, N };
    //const char* name = "ABC";

    typedef property<edge_weight_t, int> EdgeProperty;
    typedef property<vertex_index_t, int, property<vertex_distance_t, int, property<vertex_predecessor_t, int> > > VertexProperty;

    typedef adjacency_matrix<directedS, VertexProperty, EdgeProperty> Graph;

    Graph g(N);
    //EdgeWeightProperty w = 1;
    //add_edge(B, C, 3, g);
    //add_edge(C, A, 4, g);
    //add_edge(B, A, 8, g);

    /*std::cout << "vertex set: ";
    boost::print_vertices(g, name);
    std::cout << std::endl;

    std::cout << "edge set: ";
    boost::print_edges(g, name);
    std::cout << std::endl;

    std::cout << "out-edges: " << std::endl;
    boost::print_graph(g, name);
    std::cout << std::endl;

    boost::write_graphviz(std::cout, g);*/

    graph_traits<Graph>::vertex_iterator i, iend;
    property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
    //property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g);

    // Manually intialize the vertex index
    //int c = 0;
    //for (boost::tie(i, iend) = vertices(g); i != iend; ++i, ++c) {
    //    indexmap[*i] = c;
    //}

    property_map<Graph, int>::type d = get(vertex_distance, g);
    property_map<Graph, int>::type p = get(vertex_predecessor, g);

    //dijkstra_shortest_paths(g, B, d, p);


    
    return 0;
}