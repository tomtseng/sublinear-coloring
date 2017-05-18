#include <algorithm>
#include <array>
#include <vector>
#include "color.h"
#include "constants.h"
#include "graph/adj_matrix_graph.h"

using std::array;
using std::vector;

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

// fills packed_colorings with colorings, and returns the number of colorings.
// packed_colorings should be zeroed out as input.
static int GenerateColorings(array<uint32_t, kNumColors>* packed_colorings) {
  return GenerateColorings(0, 0, packed_colorings);
}

// returns index of least significant set bit in input, or -1 if no such index
// exists
static int BitBinarySearch(uint32_t bits) {
  if (bits == 0) {
    return -1;
  }
  uint32_t search_mask = 0xffff;
  int result = 0;
  for (int search_range = 16; search_range > 0;
       search_range /= 2, search_mask >>= search_range) {
    if (search_mask & bits) {
      result += search_range;
      bits >>= search_range;
    }
  }
  return result;
}

// on success, outputs a coloring into full_coloring and returns true
// on failure, returns false
bool GetColoring(const AdjMatrixGraph& graph, vector<int>* full_coloring) {
  const int n = graph.num_verts;
  const int num_chunks = (n + kChunkSize - 1) / kChunkSize;
  *full_coloring = vector<int>(n);

  // packed_colorings[i] corresponds to u_i in the writeup; it consists of
  // blocks where each block has the bitvector of i-colored vertices in some
  // coloring.
  // In order to fit everything into a uint32_t, I only consider colorings that
  // use all 3 colors and that use the 3 colors in order
  array<uint32_t, kNumColors> packed_colorings{};
  const int num_blocks = GenerateColorings(&packed_colorings);
  // B_H and B_L in the writeup; high-/low-order set bits in each block
  uint32_t high_ones = 0, low_ones = 0;
  for (int i = 0; i < num_blocks; i++) {
    high_ones <<= kBlockSize;
    high_ones |= 1 << kChunkSize;
    low_ones <<= kBlockSize;
    low_ones |= 1;
  }

  bool good_coloring = false;
  #pragma omp parallel for
  for (int chk = 0; chk < num_chunks; chk++) {
    uint32_t submatrix = graph.GetChunkSubmatrix(chk);
    // products[i] corresponds to y_i in the writeup; will hold the
    // result of multiplication of adjacency submatrix with colorings in blocks
    array<uint32_t, kNumColors> products{};
    for (int r = 0; r < kChunkSize; r++, submatrix >>= kChunkSize) {
      uint32_t row = (submatrix & kChunkMask) * low_ones;
      for (int i = 0; i < kNumColors; i++) {
        products[i] |=
          ((~(high_ones - (row & packed_colorings[i]))) & high_ones) << r;
      }
    }
    // corresponds to x in the writeup; will hold a 1 in a block if the coloring
    // in that block is valid
    uint32_t valid_flags = 0;
    for (int i = 0; i < kNumColors; i++) {
      valid_flags |= products[i] & packed_colorings[i];
    }
    valid_flags = (high_ones - valid_flags) & high_ones;

    const int one_loc = BitBinarySearch(valid_flags);
    if (one_loc < 0) {
      good_coloring = false;
    } else {
      // success, get the coloring for this chunk
      const int block_shift = one_loc - kChunkSize;
      for (int v = 0; v < kChunkSize && chk * kChunkSize + v < n; v++) {
        for (int i = 0; i < kNumColors; i++) {
          if ((1 << (block_shift + v)) & packed_colorings[i]) {
            (*full_coloring)[chk * kChunkSize + v] = chk * kNumColors + i;
            break;
          }
        }
      }
    }
  }

  return good_coloring;
}
