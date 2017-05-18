#include <fstream>
#include <string>
#include "adj_matrix_graph.h"
#include "io.h"

AdjMatrixGraph ReadAdjMatrixFromFile(const std::string& file_name) {
  long long num_verts, num_edges;
  std::ifstream fin(file_name);
  fin >> num_verts >> num_edges;
  AdjMatrixGraph graph(num_verts);
  for (long long i = 0; i < num_edges; i++) {
    int u, v;
    fin >> u >> v;
    graph.AddUndirectedEdge(u, v);
  }
  return graph;
}
