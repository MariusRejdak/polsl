#include "MatrixGenerators.h"

matrix_t generate_digraph(size_t size, int edges, int max_val)
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

matrix_t generate_graph(size_t size, int edges, int max_val)
{
	matrix_t matrix(size, std::vector<int>(size));

	for(size_t i = 0; i < size; ++i)
	{
		matrix[i][i] = 0;
		for(size_t j = i+1; j < size; ++j)
		{
			matrix[i][j] = rand()%max_val;
			matrix[j][i] = matrix[i][j];
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

matrix_t generate_dag(size_t size, unsigned int chance, int max_val)
{
	matrix_t matrix(size, std::vector<int>(size, -1));
	size_t nodes = 0;
	size_t ranks = ceil(sqrt(size));

	for (size_t i = 0; i < ranks; i++)
	{
		size_t new_nodes = size/ranks;

		for(size_t j = 0; j < nodes; ++j)
		{
			for(size_t k = 0; k < new_nodes; ++k)
			{
				if(rand()%100u < chance)
				{
					matrix[j][k+nodes] = rand()%max_val;
				}
			}
		}

		nodes += new_nodes;
	}

	return matrix;
}