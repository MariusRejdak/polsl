#include <iostream>
#include <vector>
#include <ctime>

#include "Graph.h"
#include "BoostGraphWrapper.h"

typedef std::vector<std::vector<int> > matrix_t;

matrix_t generate_graph(size_t size, int max_edges, int max_val)
{
	matrix_t matrix(size, std::vector<int>(size));

	for(size_t i = 0; i < size; ++i)
	{
		for(size_t j = 0; j < size; ++j)
		{
			matrix[i][j] = i == j ? 0 : rand()%max_val;
		}
	}

	int edges = size*(size-1);
	while(edges > max_edges)
	{
		int i = rand()%size;
		int j = rand()%size;

		if(i != j && matrix[i][j] != -1)
		{
			matrix[i][j] = -1;
			--edges;
		}
	}

	return matrix;
}

int main()
{
	matrix_t m1 = generate_graph(1000, 100000, 30);

	Graph g(m1);
	BoostGraphWrapper bg(m1);

	clock_t clockStart = clock();
	g.dijkstra(0);
	float time_g1 = ((float)clock()-clockStart)/CLOCKS_PER_SEC;
	std::cout << time_g1 << "\n";

	clockStart = clock();
	bg.dijkstra(0);
	float time_g2 = ((float)clock()-clockStart)/CLOCKS_PER_SEC;
	std::cout << time_g2 << "\n";

	return 0;
}