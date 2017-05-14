#include <iostream>
#include "graph/adj_matrix_graph.h"
#include "graph/io.h"

using std::cout;

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);

  if (argc <= 1) {
    cout << "usage: " << argv[0] << " <name of file holding input graph>\n";
    return 0;
  }

  AdjMatrixGraph graph = ReadAdjMatrixFromFile(argv[1]);

  return 0;
}
