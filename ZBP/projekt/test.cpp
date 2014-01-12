#include <iostream>
#include <vector>

#include "Graph.h"
#include "BoostGraphWrapper.h"

int main()
{
	int matrix[] = {
	//   0   1   2   3   4
		 0,  2, -1, -1, -1,
		-1,  0, -1,  1,  2,
		-1,  7,  0,  3, -1,
		-1, -1, -1,  0,  1,
		-1,  1,  1, -1,  0,
	};

	Graph g(matrix, 5);
	BoostGraphWrapper bg(matrix, 5);

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
}