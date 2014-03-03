#include <iostream>
#include <vector>
#include <ctime>
#include <memory>

#include "Graph.h"
#include "BoostGraphWrapper.h"
#include "MatrixGenerators.h"

float get_time(std::function<void()> function) {
	clock_t clockStart = clock();
	function();
	return ((float)clock()-clockStart)/CLOCKS_PER_SEC;
}

typedef struct Graph_Details
{
	size_t size;
	int edges;
	int max_val;
} Graph_Details;

void test_digraph(Graph_Details &gd)
{
	matrix_t m = generate_digraph(gd.size, gd.edges, gd.max_val);
	Graph mg(m);
	BoostGraphWrapper bg(m);
	int start_vertex = rand()%gd.size;

	std::cout << gd.size << ";" << gd.edges << ";";
	std::cout << get_time([&]() -> void {mg.dijkstra(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.dijkstra(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {mg.floyd_warshall();} ) << ";";
	std::cout << get_time([&]() -> void {bg.floyd_warshall();} ) << ";";
	std::cout << get_time([&]() -> void {mg.bellman_ford(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.bellman_ford(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {mg.bfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.bfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {mg.dfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.dfs(start_vertex);} ) << ";";

	m = generate_dag(gd.size, 40, gd.max_val);
	mg = Graph(m);
	bg = BoostGraphWrapper(m);
	std::cout << get_time([&]() -> void {mg.topological_sort();} ) << ";";
	std::cout << get_time([&]() -> void {bg.topological_sort();} ) << ";";

	m = generate_graph(gd.size, gd.edges, gd.max_val);
	mg = Graph(m);
	bg = BoostGraphWrapper(m);
	std::cout << get_time([&]() -> void {mg.kruskal();} ) << ";";
	std::cout << get_time([&]() -> void {bg.kruskal();} ) << ";";

	std::cout << std::endl;
}

int main()
{
	Graph_Details test_cases[] = {
		{ 10, 10, 50 }
	,	{ 10, 100, 50 }
	,	{ 100, 100, 50 }
	,	{ 100, 10000, 50 }
	,	{ 1000, 1000, 100 }
	,	{ 1000, 10000, 100 }
	,	{ 1000, 50000, 100 }
	,	{ 1000, 100000, 100 }
	,	{ 1000, 500000, 100 }
	,	{ 1000, 1000000, 100 }
	};

	std::cout << "vertices;edges,dijkstra_time;dijkstra_boost_time;floyd_warshall_time;floyd_warshall_boost_time;bellman_ford_time;bellman_ford_boost_time;bfs_time;bfs_boost_time;dfs_time;dfs_boost_time;topological_sort_time;topological_sort_boost_time;kruskal_time;kruskal_boost_time" << std::endl;
	for (auto gd : test_cases)
		test_digraph(gd);

	return 0;
}