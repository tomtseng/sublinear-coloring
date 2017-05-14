#ifndef GRAPH_ADJ_MATRIX_GRAPH_H_
#define GRAPH_ADJ_MATRIX_GRAPH_H_
#include <cstdint>

class AdjMatrixGraph {
 public:
  AdjMatrixGraph(unsigned _num_verts);
  ~AdjMatrixGraph();

  void AddUndirectedEdge(unsigned u, unsigned v);

 private:

  unsigned num_verts, num_edges;
  // adjacency matrix, where the k-th bit (counting from the least significant
  // bit) of adj[i][j] is set if vertex i is adjacent vertex 32 * j + k
  uint32_t** adj;
};

#endif // GRAPH_ADJ_MATRIX_GRAPH_H_
