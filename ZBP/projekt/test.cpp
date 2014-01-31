#include <iostream>
#include <vector>

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
			matrix[j][i] = -1;
			now_edges -= 2;
		}
	}

	return matrix;
}

int main()
{
	/*int matrix[] = {
	//   0   1   2   3   4
		 0,  2, -1, -1, -1,
		-1,  0, -1,  1,  2,
		-1,  7,  0,  3, -1,
		-1, -1, -1,  0,  1,
		-1,  1,  1, -1,  0,
	};*/

	srand(10);

	matrix_t matrix = generate_graph(5, 10, 30);

	Graph g(matrix);
	BoostGraphWrapper bg(matrix);

	/*for (int j = 0; j < 5; ++j)
	{
		std::cout << "For " << j << "\n";
		auto d1 = g.dijkstra(j);
		auto d2 = bg.dijkstra(j);

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}*/

	/*{
		auto d1 = g.floyd_warshall();
		auto d2 = bg.floyd_warshall();

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			for (auto j = i->begin(); j != i->end(); ++j)
				std::cout << *j << " ";
			std::cout << "\n";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			for (auto j = i->begin(); j != i->end(); ++j)
				std::cout << *j << " ";
			std::cout << "\n";
		}
		std::cout << "\n";
	}*/

	/*for (int j = 0; j < 5; ++j)
	{
		std::cout << "For " << j << "\n";
		auto d1 = g.bellman_ford(j);
		auto d2 = bg.bellman_ford(j);

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}*/

	/*for (int j = 0; j < 5; ++j)
	{
		std::cout << "For " << j << "\n";
		auto d1 = g.bfs(j);
		auto d2 = bg.bfs(j);

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}*/

	/*for (int j = 0; j < 5; ++j)
	{
		std::cout << "For " << j << "\n";
		auto d1 = g.dfs(j);
		auto d2 = bg.dfs(j);

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}*/

	/*{
		auto d1 = g.topological_sort();
		auto d2 = bg.topological_sort();

		for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";
	}*/

	{
		//auto d1 = g.kruskal();
		auto d2 = bg.kruskal();

		/*for (auto i = d1.begin(); i != d1.end(); ++i)
		{
			std::cout << *i << " ";
		}
		std::cout << "\n";*/

		for (auto i = d2.begin(); i != d2.end(); ++i)
		{
			std::cout << i->first << " " << i->second << "\n";
		}
		std::cout << "\n";
	}
}