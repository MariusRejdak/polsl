#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include <boost/graph/properties.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>

#include <boost/graph/graphviz.hpp>

using namespace boost;

class custom_dfs_visitor : public boost::default_dfs_visitor
{
public:
 
  template < typename Vertex, typename Graph >
  void finish_vertex(Vertex u, const Graph & g) const
  {
    std::cout << u << std::endl;
  }
};

class custom_bfs_visitor : public boost::default_bfs_visitor
{
public:
 
  template < typename Vertex, typename Graph >
  void finish_vertex(Vertex u, const Graph & g) const
  {
    std::cout << u << std::endl;
  }
};

int main(int, char *[])
{
  typedef adjacency_list_traits<vecS, vecS, directedS>::vertex_descriptor vertex_descriptor;
  typedef adjacency_list < vecS, vecS, directedS,
    //property<vertex_index_t, int, 
    property<vertex_distance_t, float
    //    property<vertex_color_t, default_color_type,
    //      property<vertex_predecessor_t, vertex_descriptor>
    //    > 
    //  >
    >, 
    property<edge_weight_t, float> 
  > graph_t;
  typedef std::pair<int, int> Edge;

  typedef exterior_vertex_property<graph_t, float> distance_property_t;
  typedef distance_property_t::matrix_type distance_matrix_t;
  typedef distance_property_t::matrix_map_type distance_matrix_map_t;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  Edge edge_array[] = { 
    Edge(0, 1), 
    Edge(1, 3), 
    Edge(1, 4),
    Edge(2, 1), 
    Edge(2, 3), 
    Edge(3, 4), 
    Edge(4, 1), 
    Edge(4, 2)
  };
  float weights[] = { 2, 1, 2, 7, 3, 1, 1, 1 };

  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  graph_traits<graph_t>::vertex_iterator i, iend;

  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);

  // Manually intialize the vertex index and name maps
  //property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);

  graph_traits <graph_t>::vertex_iterator vi, vend;

  vertex_descriptor s = vertex(A, g);

  write_graphviz(std::cout, g);

  property_map<graph_t, vertex_distance_t>::type d = get(vertex_distance, g);
  //property_map<graph_t, vertex_predecessor_t>::type p = get(vertex_predecessor, g);
  //property_map<graph_t, vertex_color_t>::type cm = get(vertex_color, g);

  
  distance_matrix_t distances(num_vertices(g));
  distance_matrix_map_t dm(distances, g);
  //dijkstra_shortest_paths(g, s, distance_map(d));
  floyd_warshall_all_pairs_shortest_paths(g, dm);

  /*std::cout << "distances and parents:" << std::endl;
  
  for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distance(" << *vi << ") = " << d[*vi] << ", ";
    //std::cout << "parent(" << indexmap[*vi] << ") = " << indexmap[p[*vi]] << std::endl;
  }
  std::cout << std::endl;*/

  std::cout << "distances:" << std::endl;
  for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distances(" << *vi << ") =";

    graph_traits <graph_t>::vertex_iterator vix, vendx;
    for (tie(vix, vendx) = vertices(g); vix != vendx; ++vix) {
      std::cout << " " << dm[*vi][*vix];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  custom_bfs_visitor vis;
  breadth_first_search(g, s, visitor(vis));
  //custom_dfs_visitor vis;
  //depth_first_search(g, root_vertex(s).visitor(vis));

  std::cout << "visited:" << std::endl;
  for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "visited(" << *vi << ") = " << *vi << std::endl;
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}