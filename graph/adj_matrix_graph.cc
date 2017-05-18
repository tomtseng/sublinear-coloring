#include <algorithm>
#include "adj_matrix_graph.h"
#include "constants.h"

// Number of bits of one "word" (uint32_t)
const int kWordWidth = 32;

static inline int RoundUp(int num, int granularity) {
  return (num + granularity - 1) / granularity * granularity;
}

// Initializes adj. Fills with zeroes if init_vals is nullptr, otherwise copies
// values fomr init_vals into adj
void AdjMatrixGraph::InitAdj(uint32_t** init_vals) {
  // round num_verts up to nearest multiple of kChunkSize. This is to avoid
  // out-of-bounds problems in GetChunkSubmatrix without needing annoying bounds
  // checking
  const int rounded_num_verts = RoundUp(num_verts, kChunkSize);
  const int num_vecs = (rounded_num_verts + kWordWidth - 1) / kWordWidth;
  const int arr_len = rounded_num_verts * num_vecs;
  uint32_t* arr = new uint32_t[arr_len];
  adj = new uint32_t*[rounded_num_verts];
  #pragma omp parallel for
  for (int i = 0; i < rounded_num_verts; i++) {
    adj[i] = &arr[i * num_vecs];
  }
  if (init_vals == nullptr) {
    std::fill(adj[0], adj[0] + arr_len, 0);
  } else {
    std::copy(init_vals[0], init_vals[0] + arr_len, adj[0]);
  }
}

AdjMatrixGraph::AdjMatrixGraph(int _num_verts) :
  num_verts(_num_verts) {
  InitAdj();
}

AdjMatrixGraph::~AdjMatrixGraph() {
  delete[] adj[0];
  delete[] adj;
}

AdjMatrixGraph::AdjMatrixGraph(const AdjMatrixGraph& other) :
  num_verts(other.num_verts) {
  InitAdj(other.adj);
}

AdjMatrixGraph& AdjMatrixGraph::operator=(const AdjMatrixGraph& other) {
  if (this != &other) {
    this->~AdjMatrixGraph();
    num_verts = other.num_verts;
    InitAdj(other.adj);
  }
  return *this;
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
