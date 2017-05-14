#ifndef GRAPH_IO_H_
#define GRAPH_IO_H_
#include <string>
#include "adj_matrix_graph.h"

// Reads graph from file. File format is as follows:
//   <number of vertices> <number of edges>
// followed by <number of edges> many pairs of numbers in the range
// [0 .. <number of vertices> - 1]
//   <u> <v>
// indicating that there is an edge between vertices u and v in the graph.
AdjMatrixGraph ReadAdjMatrixFromFile(const std::string& file_name);

#endif // GRAPH_IO_H_
