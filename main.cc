#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>
#include "color.h"
#include "graph/adj_matrix_graph.h"
#include "graph/io.h"

using std::cout;

#define DEBUG

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);

  if (argc <= 1) {
    cout << "usage: " << argv[0] << " <name of file holding input graph>\n";
    return 0;
  }

  AdjMatrixGraph graph = ReadAdjMatrixFromFile(argv[1]);
  std::clock_t start = std::clock();
  std::vector<int> coloring;
  if (!GetColoring(graph, &coloring)) {
    cout <<
      "Coloring failed. Are you sure the input graph is 3-colorable?\n";
    return 0;
  }
  double duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
  cout << "Time elapsed: " << duration << " seconds" << std::endl;

  #ifdef DEBUG
  const int num_verts = graph.num_verts;
  bool bad_coloring = false;
  #pragma omp parallel for
  for (int i = 0; i < num_verts; i++) {
    #pragma omp parallel for
    for (int j = i + 1; j < num_verts; j++) {
      if (graph.GetEdge(i, j) && coloring[i] == coloring[j]) {
        bad_coloring = true;
      }
    }
  }
  if (bad_coloring) {
    cout << "DEBUG: BAD COLORING\n";
  }
  #endif

  std::sort(coloring.begin(), coloring.end());
  const int num_colors =
    std::unique(coloring.begin(), coloring.end()) - coloring.begin();
  cout << "Used " << num_colors << " colors for " << num_verts << " vertices\n";

  return 0;
}
