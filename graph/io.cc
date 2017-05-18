#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "adj_matrix_graph.h"
#include "io.h"

using std::ifstream;
using std::string;

AdjMatrixGraph ReadAdjMatrixFromFile(const string& file_name) {
  long long num_verts, num_edges;
  ifstream fin(file_name);
  fin >> num_verts >> num_edges;
  if (num_verts > std::numeric_limits<int>::max()) {
    std::cerr << "Oops, this can't handle graphs that large.\n";
    return AdjMatrixGraph(0);
  }
  AdjMatrixGraph graph(num_verts);
  for (long long i = 0; i < num_edges; i++) {
    int u, v;
    fin >> u >> v;
    graph.AddUndirectedEdge(u, v);
  }
  return graph;
}
