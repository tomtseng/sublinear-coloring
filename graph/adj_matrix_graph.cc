#include <algorithm>
#include "adj_matrix_graph.h"
#include "constants.h"

// Number of bits of one "word" (uint32_t)
const unsigned kWordWidth = 32;

AdjMatrixGraph::AdjMatrixGraph(unsigned _num_verts) :
  num_verts(_num_verts),
  max_chunk_idx((num_verts + kChunkSize - 1) / kChunkSize) {
  // round num_verts to nearest multiple of kChunkSize. This is to avoid
  // out-of-bounds problems in GetChunkSubmatrix without needing annoying bounds checking
  const unsigned rounded_num_verts =
    (num_verts + kChunkSize - 1) / kChunkSize * kChunkSize;
  const unsigned num_vecs = (rounded_num_verts + kWordWidth - 1) / kWordWidth;
  uint32_t* arr = new uint32_t[rounded_num_verts * num_vecs];
  adj = new uint32_t*[rounded_num_verts];
  #pragma omp parallel for
  for (unsigned i = 0; i < rounded_num_verts; i++) {
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

void AdjMatrixGraph::AddUndirectedEdge(unsigned u, unsigned v) {
  adj[u][v / kWordWidth] |= 1 << (v % kWordWidth);
  adj[v][u / kWordWidth] |= 1 << (u % kWordWidth);
}

bool AdjMatrixGraph::GetEdge(unsigned u, unsigned v) const {
  return (adj[u][v / kWordWidth] >> (v % kWordWidth)) & 1;
}

unsigned AdjMatrixGraph::GetChunkSubmatrix(unsigned chunk_idx) const {
  unsigned submatrix = 0;
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
