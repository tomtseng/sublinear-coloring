#ifndef GRAPH_ADJ_MATRIX_GRAPH_H_
#define GRAPH_ADJ_MATRIX_GRAPH_H_
#include <cstdint>

class AdjMatrixGraph {
 public:
  AdjMatrixGraph(unsigned _num_verts);
  ~AdjMatrixGraph();

  void AddUndirectedEdge(unsigned u, unsigned v);
  // Return true if there is an edge between vertex u and vertex v
  bool GetEdge(unsigned u, unsigned v) const;
  // Get adjacency matrix for a chunk
  // Returns bitvector where if vertex i in the chunk is adjacenct to vertex j
  // in the chunk, then the (kChunkSize * i + j)-th bit (counting from LSB) is
  // set
  unsigned GetChunkSubmatrix(unsigned chunk_idx) const;

  const unsigned num_verts;

 private:
  // adjacency matrix, where the k-th bit (counting from LSB) of adj[i][j] is
  // set if vertex i is adjacent vertex kVecWidth * j + k
  uint32_t** adj;
};

#endif // GRAPH_ADJ_MATRIX_GRAPH_H_
