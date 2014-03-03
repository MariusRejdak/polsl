#ifndef MATRIXGENERATORS_H_INCLUDED
#define MATRIXGENERATORS_H_INCLUDED

#include <vector>
#include <memory>
#include <cmath>

typedef std::vector<std::vector<int> > matrix_t;

matrix_t generate_digraph(size_t size, int edges, int max_val);
matrix_t generate_graph(size_t size, int edges, int max_val);
matrix_t generate_dag(size_t size, unsigned int chance, int max_val);

#endif // MATRIXGENERATORS_H_INCLUDED