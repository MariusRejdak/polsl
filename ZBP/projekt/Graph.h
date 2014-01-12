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
    std::vector<int> bfs(unsigned int start_vertex);
    std::vector<int> dfs(unsigned int start_vertex);
private:
	std::vector<std::vector<int> > matrix;
	size_t m_size;

	class Diskstra_cmp
	{
	public:
		Diskstra_cmp(std::vector<int> &d) : d(d)
		{
			//Empty
		}

		bool operator() (const int& lhs, const int&rhs) const
		{
			if (d[lhs] != -1 && d[rhs] != -1)
				return d[lhs] > d[rhs];
			else if(d[lhs] != -1)
				return true;
			else
				return false;
		}
	private:
		std::vector<int> &d;
	};

};

#endif // GRAPH_H_INCLUDED
