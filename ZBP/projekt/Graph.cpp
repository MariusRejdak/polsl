#include "Graph.h"

#include <vector>
#include <climits>
#include <iostream>
#include <queue>
#include <functional>
#include <algorithm>

Graph::Graph(const int *matrix, size_t size) : matrix(std::vector<std::vector<int> >(size, std::vector<int>(size))), m_size(size)
{
	for (size_t i = 0; i < m_size; ++i)
	{
		for (size_t j = 0; j < m_size; ++j)
		{
			this->matrix[i][j] = (matrix[i*m_size+j] < 0 || matrix[i*m_size+j] == INT_MAX) ? -1 : matrix[i*m_size+j];
		}
	}
}

Graph::Graph(const std::vector<std::vector<int> > &matrix) : matrix(matrix), m_size(matrix.size())
{
	//Empty
}

std::vector<int> Graph::dijkstra(unsigned int start_vertex)
{
	std::vector<int> d(m_size, -1);
	auto Q_comp = [&](const int &lhs, const int &rhs) -> bool
	{
		if ((d[lhs] == -1) && (d[rhs] == -1))
			return false;
		else if(d[lhs] == -1)
			return true;
		else if(d[rhs] == -1)
			return false;
		else
			return (d[lhs] > d[rhs]);
	};
	std::vector<int> Q;

	for (size_t i = 0; i < m_size; ++i)
		Q.push_back(i);

	d[start_vertex] = 0;
	std::make_heap(Q.begin(), Q.end(), Q_comp);

	while(!Q.empty())
	{
		int u = Q.front();

		std::pop_heap(Q.begin(), Q.end(), Q_comp);
		Q.pop_back();

		for (size_t v = 0; v < m_size; ++v)
		{
			if(matrix[u][v] != -1)
			{
				if(((d[v] > d[u] + matrix[u][v]) && (d[v] != -1) && (d[u] != -1))
					|| ((d[v] == -1) && (d[u] != -1)))
				{
					d[v] = d[u] + matrix[u][v];
				}
			}
		}
	}
	return d;
}

std::vector<std::vector<int> > Graph::floyd_warshall()
{
	std::vector<std::vector<int> > d = matrix;

	for (size_t i = 0; i != m_size; ++i)
		d[i][i] = 0;

	for(size_t k = 0; k < m_size; ++k)
	{
		for(size_t i = 0; i < m_size; ++i)
		{
			if(d[i][k] != -1)
			{
				for (size_t j = 0; j < m_size; ++j)
				{
					if(d[i][j] != -1 && d[k][j] != -1)
					{
						if(d[i][j] > d[i][k] + d[k][j])
						{
							d[i][j] = d[i][k] + d[k][j];
						}
					}
					else if(d[k][j] != -1)
					{
						d[i][j] = d[i][k] + d[k][j];
					}
				}
			}
		}
	}
	return d;
}

std::vector<int> Graph::bfs(unsigned int start_vertex)
{
	int counter = 0;
	std::vector<int> output(m_size);
	std::queue<int> Q;
	std::vector<short> visited(m_size, 0);

	visited[start_vertex] = 1;
	Q.push(start_vertex);

	while(!Q.empty())
	{
		size_t u = Q.front();
		Q.pop();
		for(size_t v = 0; v < m_size; ++v)
		{
			if((matrix[u][v] != -1) && (v != u))
			{
				if(visited[v] == 0)
				{
					visited[v] = 1;
					Q.push(v);
				}
			}
		}
		visited[u] = 2;
		output[u] = counter++;
	}

	return output;
}

std::vector<int> Graph::dfs(unsigned int start_vertex)
{
	int counter = 0;
	std::vector<int> output(m_size);
	std::vector<short> visited(m_size, 0);

	std::function<void(unsigned u)> dfs_visit = [&](unsigned int u)
	{
		visited[u] = 1;
		for(size_t v = 0; v < m_size; ++v)
		{
			if((matrix[u][v] != -1) && (u != v) && (visited[v] == 0))
			{
				dfs_visit(v);
			}
		}
		visited[u] = 2;
		output[u] = counter++;
	};

	dfs_visit(start_vertex);
	for(size_t v = 0; v < m_size; ++v)
	{
		if(visited[v] == 0)
			dfs_visit(v);
	}

	return output;
}
