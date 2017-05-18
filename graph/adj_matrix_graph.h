#ifndef GRAPH_ADJ_MATRIX_GRAPH_H_
#define GRAPH_ADJ_MATRIX_GRAPH_H_
#include <cstdint>

class AdjMatrixGraph {
 public:
  AdjMatrixGraph(int _num_verts);
  ~AdjMatrixGraph();

  void AddUndirectedEdge(int u, int v);
  // Return true if there is an edge between vertex u and vertex v
  bool GetEdge(int u, int v) const;
  // Get adjacency matrix for a chunk
  // Returns bitvector where if vertex i in the chunk is adjacent to vertex j
  // in the chunk, then the (kChunkSize * i + j)-th bit (counting from LSB) is
  // set
  uint32_t GetChunkSubmatrix(int chunk_idx) const;

  const int num_verts;

 private:
  // adjacency matrix, where the k-th bit (counting from LSB) of adj[i][j] is
  // set if vertex i is adjacent vertex kVecWidth * j + k
  uint32_t** adj;
};

#endif // GRAPH_ADJ_MATRIX_GRAPH_H_
