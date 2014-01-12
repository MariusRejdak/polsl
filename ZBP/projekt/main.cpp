#include <iostream>
#include <vector>
#include <ctime>

#include "Graph.h"
#include "BoostGraphWrapper.h"

td::vector<std::vector<int> > generate_graph(size_t size, int max_edges, int max_val)
{
	std::vector<std::vector<int> > matrix(size, std::vector<int>(size));

	for(size_t i = 0; i < size; ++i)
	{
		for(size_t j = 0; j < size; ++j)
		{
			matrix[i][j] = i == j ? 0 : rand()%max_val;
		}
	}
}

int main()
{
	return 0;
}