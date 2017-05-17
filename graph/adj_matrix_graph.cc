#include <algorithm>
#include "adj_matrix_graph.h"
#include "constants.h"

// Number of bits of one "word" (uint32_t)
const int kWordWidth = 32;

AdjMatrixGraph::AdjMatrixGraph(int _num_verts) :
  num_verts(_num_verts) {
  // round num_verts to nearest multiple of kChunkSize. This is to avoid
  // out-of-bounds problems in GetChunkSubmatrix without needing annoying bounds checking
  const int rounded_num_verts =
    (num_verts + kChunkSize - 1) / kChunkSize * kChunkSize;
  const int num_vecs = (rounded_num_verts + kWordWidth - 1) / kWordWidth;
  uint32_t* arr = new uint32_t[rounded_num_verts * num_vecs];
  adj = new uint32_t*[rounded_num_verts];
  #pragma omp parallel for
  for (int i = 0; i < rounded_num_verts; i++) {
    adj[i] = &arr[i * num_vecs];
    std::fill(adj[i], adj[i] + num_vecs, 0);
  }
}

AdjMatrixGraph::~AdjMatrixGraph() {
  if (adj != nullptr) {
    delete[] adj[0];
    delete[] adj;
  }
}

void AdjMatrixGraph::AddUndirectedEdge(int u, int v) {
  adj[u][v / kWordWidth] |= 1 << (v % kWordWidth);
  adj[v][u / kWordWidth] |= 1 << (u % kWordWidth);
}

bool AdjMatrixGraph::GetEdge(int u, int v) const {
  return (adj[u][v / kWordWidth] >> (v % kWordWidth)) & 1;
}

uint32_t AdjMatrixGraph::GetChunkSubmatrix(int chunk_idx) const {
  uint32_t submatrix = 0;
  const int vOffset = chunk_idx * kChunkSize;
  for (int i = kChunkSize - 1; i >= 0; i--) {
    // XXX It happens to be nice to fetch a row of the submatrix here because
    // kChunkSize divides kWordWidth; a row fits inside a word. However, in
    // general, a row of the submatrix may be split across two words.
    submatrix = (submatrix << kChunkSize) |
      ((adj[vOffset + i][vOffset / kWordWidth] >> (vOffset % kWordWidth))
       & kChunkMask);
  }
  return submatrix;
}
