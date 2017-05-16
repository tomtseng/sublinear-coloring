#include <vector>
#include "color.h"
#include "constants.h"
#include "graph/adj_matrix_graph.h"

using std::vector;

vector<unsigned> GetColoring(const AdjMatrixGraph& graph) {
  const int n = graph.num_verts;
  const int num_chunks = (n + kChunkSize - 1) / kChunkSize;
  vector<unsigned> coloring(n);
  // TODO
  return coloring;
}
