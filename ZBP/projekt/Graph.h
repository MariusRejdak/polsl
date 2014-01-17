#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <iostream>

class Graph {
public:
	Graph(const int *matrix, size_t size);
	Graph(const std::vector<std::vector<int> > &matrix);
	std::vector<int> dijkstra(unsigned int start_vertex);
	std::vector<std::vector<int> > floyd_warshall();
	std::vector<int> bellman_ford(int start_vertex);
	std::vector<int> bfs(unsigned int start_vertex);
	std::vector<int> dfs(unsigned int start_vertex);
	std::vector<int> topological_sort();
private:
	std::vector<std::vector<int> > matrix;
	size_t m_size;
};

#endif // GRAPH_H_INCLUDED
