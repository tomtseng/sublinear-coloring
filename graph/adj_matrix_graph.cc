#include "adj_matrix_graph.h"

AdjMatrixGraph::AdjMatrixGraph(unsigned _num_verts) :
  num_verts(_num_verts), num_edges(0) {
  adj = new uint32_t*[num_verts];
  const unsigned num_vecs = (num_verts + 32 - 1) / 32;
  #pragma omp parallel for
  for (unsigned i = 0; i < num_verts; i++) {
    adj[i] = new uint32_t[num_vecs];
    for (unsigned j = 0; j < num_vecs; j++) {
      adj[i][j] = 0;
    }
  }
}

void AdjMatrixGraph::AddUndirectedEdge(unsigned u, unsigned v) {
  adj[u][v / 32] |= 1 << (v % 32);
  adj[v][u / 32] |= 1 << (u % 32);
}

AdjMatrixGraph::~AdjMatrixGraph() {
  if (adj != nullptr) {
    for (unsigned i = 0; i < num_edges; i++) {
      delete adj[i];
    }
    delete adj;
  }
}
