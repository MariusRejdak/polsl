#include "BoostGraphWrapper.h"

#include <vector>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

using namespace boost;

template <typename CounterMap>
class dfs_counter_visitor: public default_dfs_visitor
{
public:
    template <typename Graph>
    dfs_counter_visitor(const Graph &g, CounterMap cm) : counter(0), d(cm)
    {
        //Empty
    }
   
    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph &g)
    {
        std::cout << u << " " << counter << std::endl;
        put(d, u, counter++);
    }

private:
    int counter;
    CounterMap d;
};

template <typename CounterMap>
class bfs_counter_visitor: public default_bfs_visitor
{
public:
    template <typename Graph>
    bfs_counter_visitor(const Graph &g, CounterMap cm) : counter(0), d(cm)
    {
        //Empty
    }
   
    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph &g)
    {
        std::cout << u << " " << counter << std::endl;
        put(d, u, counter++);
    }

private:
    int counter;
    CounterMap d;
};

BoostGraphWrapper::BoostGraphWrapper(int *matrix, size_t size) : g(size)
{
    vertex_iterator vi, vj, vend;
    tie(vi, vend) = vertices(g);
    for(size_t i = 0; i < size; ++i, ++vi)
    {
        tie(vj, vend) = vertices(g);
        for (size_t j = 0; j < size; ++j, ++vj)
        {
            if(matrix[i*size+j] > 0)
                add_edge(*vi, *vj, matrix[i*size+j], g);
        }
    }
}

BoostGraphWrapper::BoostGraphWrapper(std::vector<std::vector<int> > matrix) : g(matrix.size())
{
    vertex_iterator vi, vj, vend;
    tie(vi, vend) = vertices(g);
    for(size_t i = 0; i < matrix.size(); ++i, ++vi)
    {
        tie(vj, vend) = vertices(g);
        for (size_t j = 0; j < matrix[i].size(); ++j, ++vj)
        {
            if(matrix[i][j] > 0)
                add_edge(*vi, *vj, matrix[i][j], g);
        }
    }
}

std::vector<int> BoostGraphWrapper::dijkstra(int start_vertex)
{
    graph_distance_map d = get(vertex_distance, g);
    vertex_descriptor s = vertex(start_vertex, g);
    std::vector<int> output(num_vertices(g));

    dijkstra_shortest_paths(g, s, distance_map(d));

    vertex_iterator vi, vend;
    for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
        output[*vi] = d[*vi];
    }

    return output;
}

std::vector<std::vector<int> > BoostGraphWrapper::floyd_warshall()
{
    ext_distance_matrix_t d(num_vertices(g));
    ext_distance_matrix_map_t dm(d, g);
    std::vector<std::vector<int> > output(num_vertices(g), std::vector<int>(num_vertices(g)));

    floyd_warshall_all_pairs_shortest_paths(g, dm);

    vertex_iterator vi, vj, vend;
    for (tie(vi, vend) = vertices(g); vi != vend; ++vi)
    {
        for (tie(vj, vend) = vertices(g); vj != vend; ++vj) {
            output[*vi][*vj] = dm[*vi][*vj];
        }
    }

    return output;
}

std::vector<int> BoostGraphWrapper::bfs(int start_vertex)
{
    vertex_descriptor s = vertex(start_vertex, g);
    std::vector<int> counter(num_vertices(g));
    typedef iterator_property_map<std::vector<int>::iterator, property_map<graph_t, vertex_index_t>::const_type> counter_pm_type;
    counter_pm_type dcounter_pm(counter.begin(), get(vertex_index, g));
    bfs_counter_visitor<counter_pm_type> vis(g, dcounter_pm);
    breadth_first_search(g, s, visitor(vis));
    return counter;
}

std::vector<int> BoostGraphWrapper::dfs(int start_vertex)
{
    
    vertex_descriptor s = vertex(start_vertex, g);
    std::vector<int> counter(num_vertices(g));
    typedef iterator_property_map<std::vector<int>::iterator, property_map<graph_t, vertex_index_t>::const_type> counter_pm_type;
    counter_pm_type dcounter_pm(counter.begin(), get(vertex_index, g));
    dfs_counter_visitor<counter_pm_type> vis(g, dcounter_pm);
    depth_first_search(g, root_vertex(s).visitor(vis));
    return counter;
}

void BoostGraphWrapper::print(std::ostream& o)
{
    write_graphviz(o, g);
}