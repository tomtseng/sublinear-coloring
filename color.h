#ifndef COLOR_H_
#define COLOR_H_
#include <vector>
#include "graph/adj_matrix_graph.h"

bool GetColoring(const AdjMatrixGraph& graph, std::vector<int>* coloring);

#endif // COLOR_H_
