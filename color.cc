#include <algorithm>
#include <array>
#include <vector>
#include "color.h"
#include "constants.h"
#include "graph/adj_matrix_graph.h"

using std::array;
using std::vector;

const static int kInf = 10000;
const static int kNumColors = 3;
const static int kBlockSize = kChunkSize + 1;

// Scratch space as a global, because I like having bad code
static int tmp_coloring[kChunkSize];

static int GenerateColorings(int cur_idx, int max_color,
    array<uint32_t, kNumColors>* packed_colorings) {
  if (cur_idx == kChunkSize) { // Done generating a coloring
    if (max_color == kNumColors) { // Coloring uses all colors
      // Add the coloring to packed_colorings
      for (int i = 0; i < kNumColors; i++) {
        (*packed_colorings)[i] <<= kBlockSize;
      }
      for (int i = 0; i < kChunkSize; i++) {
        (*packed_colorings)[tmp_coloring[i]] |= 1 << i;
      }
      return 1;
    } else {
      return 0;
    }
  } else {
    int num_colorings = 0;
    for (int i = 0; i < std::min(max_color + 1, kNumColors); i++) {
      tmp_coloring[cur_idx] = i;
      num_colorings += GenerateColorings(cur_idx + 1,
          std::max(std::min(i + 1, kNumColors), max_color), packed_colorings);
    }
    return num_colorings;
  }
}

// fills packed_colorings with colorings, and returns the number of colorings
static int GenerateColorings(array<uint32_t, kNumColors>* packed_colorings) {
  return GenerateColorings(0, 0, packed_colorings);
}

vector<int> GetColoring(const AdjMatrixGraph& graph) {
  const int n = graph.num_verts;
  const int num_chunks = (n + kChunkSize - 1) / kChunkSize;
  vector<int> coloring(n);

  // packed_colorings[i] corresponds to u_i in the writeup; it consists of
  // blocks where each block has the bitvector of i-colored vertices in some
  // coloring.
  // In order to fit everything into a uint32_t, I only consider colorings that
  // use all 3 colors and that use the 3 colors in order
  array<uint32_t, kNumColors> packed_colorings;
  const int num_blocks = GenerateColorings(&packed_colorings);
  // B_L and B_R in the writeup; left/right-aligned 1's in each block
  uint32_t left_ones = 0, right_ones = 0;
  for (int i = 0; i < num_blocks; i++) {
    left_ones <<= kBlockSize;
    left_ones |= 1 << kChunkSize;
    right_ones <<= kBlockSize;
    right_ones |= 1;
  }

  #pragma omp parallel for
  for (int i = 0; i < num_chunks; i++) {
    // TODO
  }
  return coloring;
}
