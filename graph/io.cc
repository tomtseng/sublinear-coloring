#include <fstream>
#include <string>
#include "adj_matrix_graph.h"
#include "io.h"

using std::ifstream;
using std::string;

AdjMatrixGraph ReadAdjMatrixFromFile(const string& file_name) {
  unsigned num_verts, num_edges;
  ifstream fin(file_name);
  fin >> num_verts >> num_edges;
  AdjMatrixGraph graph(num_verts);
  for (unsigned i = 0; i < num_edges; i++) {
    int u, v;
    fin >> u >> v;
    graph.AddUndirectedEdge(u, v);
  }
  return graph;
}
