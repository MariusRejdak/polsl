#include <iostream>
#include <vector>
#include <ctime>
#include <memory>

#include "Graph.h"
#include "BoostGraphWrapper.h"

typedef std::vector<std::vector<int> > matrix_t;

matrix_t generate_graph(size_t size, int edges, int max_val)
{
	matrix_t matrix(size, std::vector<int>(size));

	for(size_t i = 0; i < size; ++i)
	{
		for(size_t j = 0; j < size; ++j)
		{
			matrix[i][j] = i == j ? 0 : rand()%max_val;
		}
	}

	int now_edges = size*(size-1);
	while(now_edges > edges)
	{
		int i = rand()%size;
		int j = rand()%size;

		if(i != j && matrix[i][j] != -1)
		{
			matrix[i][j] = -1;
			--now_edges;
		}
	}

	return matrix;
}

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

void test_graph(Graph_Details &gd)
{
	matrix_t m = generate_graph(gd.size, gd.edges, gd.max_val);
	Graph mg(m);
	BoostGraphWrapper bg(m);
	int start_vertex = rand()%gd.size;

	std::cout << gd.size << ";" << gd.edges << ";";
	std::cout << get_time([&]() -> void {mg.dijkstra(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.dijkstra(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {mg.floyd_warshall();} ) << ";";
	std::cout << get_time([&]() -> void {bg.floyd_warshall();} ) << ";";
	std::cout << get_time([&]() -> void {mg.bfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.bfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {mg.dfs(start_vertex);} ) << ";";
	std::cout << get_time([&]() -> void {bg.dfs(start_vertex);} );
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

	std::cout << "vertices;edges,dijkstra_time;dijkstra_boost_time;floyd_warshall_time;floyd_warshall_boost_time;bfs_time;bfs_boost_time;dfs_time;dfs_boost_time" << std::endl;
	for (auto gd : test_cases)
		test_graph(gd);

	return 0;
}