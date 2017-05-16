#ifndef COLOR_H_
#define COLOR_H_
#include "graph/adj_matrix_graph.h"

#include <vector>

std::vector<unsigned> GetColoring(const AdjMatrixGraph& graph);

#endif // COLOR_H_
