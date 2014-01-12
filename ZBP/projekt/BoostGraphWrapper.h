#ifndef BOOSTGRAPHWRAPPER_H_INCLUDED
#define BOOSTGRAPHWRAPPER_H_INCLUDED

#include <vector>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/properties.hpp>

class BoostGraphWrapper {
public:
	BoostGraphWrapper(int *matrix, size_t size);
	BoostGraphWrapper(std::vector<std::vector<int> > &matrix);
	std::vector<int> dijkstra(int start_vertex);
	std::vector<std::vector<int> > floyd_warshall();
	std::vector<int> bfs(int start_vertex);
	std::vector<int> dfs(int start_vertex);
	void print(std::ostream& o);
private:
	typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>::vertex_descriptor vertex_descriptor;
	typedef boost::property<boost::edge_weight_t, int> edge_property;
	typedef boost::property<boost::vertex_distance_t, int> vertex_property;
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_property, edge_property> graph_t;
	typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;
	typedef boost::exterior_vertex_property<graph_t, int> ext_distance_property_t;
	typedef ext_distance_property_t::matrix_type ext_distance_matrix_t;
	typedef ext_distance_property_t::matrix_map_type ext_distance_matrix_map_t;
	typedef boost::property_map<graph_t, boost::vertex_distance_t>::type graph_distance_map;

	graph_t g;
};

#endif // BOOSTGRAPHWRAPPER_H_INCLUDED