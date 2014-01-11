//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include <boost/graph/breadth_first_search.hpp>

#include <boost/graph/depth_first_search.hpp>

using namespace boost;

int
main(int, char *[])
{
  typedef adjacency_list_traits<listS, listS, directedS>::vertex_descriptor vertex_descriptor;
  typedef adjacency_list < listS, listS, directedS,
    property<vertex_index_t, int, property<vertex_distance_t, float, property<vertex_predecessor_t, vertex_descriptor> > >, 
    property<edge_weight_t, float> 
  > graph_t;
  typedef std::pair<int, int> Edge;

  typedef exterior_vertex_property<graph_t, float> distance_property_t;
  typedef distance_property_t::matrix_type distance_matrix_t;
  typedef distance_property_t::matrix_map_type distance_matrix_map_t;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };
  float weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  graph_traits<graph_t>::vertex_iterator i, iend;

  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);

  // Manually intialize the vertex index and name maps
  property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);
  int c = 0;
  for (boost::tie(i, iend) = vertices(g); i != iend; ++i, ++c) {
    indexmap[*i] = c;
  }

  vertex_descriptor s = vertex(A, g);

  property_map<graph_t, vertex_distance_t>::type d = get(vertex_distance, g);
  property_map<graph_t, vertex_predecessor_t>::type p = get(vertex_predecessor, g);
  dijkstra_shortest_paths(g, s, predecessor_map(p).distance_map(d));

  distance_matrix_t distances(num_vertices(g));
  distance_matrix_map_t dm(distances, g);
  floyd_warshall_all_pairs_shortest_paths(g, dm);

  std::cout << "distances and parents:" << std::endl;
  graph_traits <graph_t>::vertex_iterator vi, vend;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distance(" << indexmap[*vi] << ") = " << d[*vi] << ", ";
    std::cout << "parent(" << indexmap[*vi] << ") = " << indexmap[p[*vi]] << std::endl;
  }
  std::cout << std::endl;

  std::cout << "distances:" << std::endl;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distances(" << indexmap[*vi] << ") =";

    graph_traits <graph_t>::vertex_iterator vix, vendx;
    for (boost::tie(vix, vendx) = vertices(g); vix != vendx; ++vix) {
      std::cout << " " << dm[*vi][*vix];
    }
    std::cout << std::endl;
  }

  breadth_first_search(g, s);
  //depth_first_search(g, s);

  return EXIT_SUCCESS;
}