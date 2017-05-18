# o(n)-coloring 3-colorable graphs in O(n) time

### Summary

Suppose we're given a 3-colorable graph, and we want to give this graph a vertex
coloring. In general, we cannot hope to give an optimal coloring unless P = NP.
Instead we look for approximation algorithms that give a good coloring in
polynomial time. In 2014, Kawarabayashi and Thorup gave an algorithm that finds
an O(n^0.19996) coloring in polynomial time. This is the latest in a long
sequence of works that give O(n^c) colorings for steadily decreasing values of
c.

An interesting extension that has no use in either theory or practice is
to stipulate a stronger running time requirement. In particular, we wonder what
the best coloring achievable is using O(n) running time, where n is the number
of vertices in the graph. This means that if the graph is dense, we cannot even
afford to look at all the edges of the graph. Is it possible to color the graph
using o(n) colors?

In this project, we show that in the word RAM model, we can color a 3-colorable
graph with O(n / log log n) colors in O(n) time.

### Algorithm

See writeup/writeup.pdf for details.

The high-level idea is as follows: we break the vertices into chunks of size k,
and for the subgraph induced by each chunk, we 3-color the subgraph by brute
force (using a different set of 3 colors for each subgraph). This gives a 3n/k
coloring of the graph. We set k to be just a little bigger than a constant. To
keep the runtime linear, we use some bit hackery to get some word-level
parallelism.

### Running the code

For fun (assuming a loose definition of "fun"), this repository also contains
some proof-of-concept code for the algorithm. Type `make` and then `./run <input
graph file>`, e.g. `./run inputs/petersen.txt` to confirm for yourself that the
code does indeed run without exploding immediately.

The input format can be seen in graph/io.h. There's a simple graph generator in
inputs/gen.py. It's really slow because it's written in Python and because it iterates over
O(n^2) pairs of vertices.

Unlike in the word RAM model, we don't have computers that actually scale their
word size to the input size. Instead, the code always treats `uint32_t` as a
"word," i.e. the word size is 32 bits. With this, we output 3n/4-colorings
of 3-colorable graphs. Are you impressed yet?

As a bonus, this code uses OpenMP to demonstrate that the algorithm has great
parallelism. The coloring for each chunk can be found in parallel. Except you
can't get any noticeable speedup because (1) all the running time is spent
reading in the input and (2) you can't run this code on any respectably large
graph because storing a graph as an adjacency matrix floods your machine's
memory.

### Open questions

* This algorithm crucially relies on the word RAM model. Can we achieve an o(n)
  coloring without the bit hackery?
* Is it possible to achieve a truly sublinear coloring, i.e. an O(n^(1 - c))
  coloring for some c > 0?
* What lower bounds can we prove assuming this O(n) running time restriction?

### Acknowledgments

This question was posed by some members of the Spring 2017
[15-251](http://www.cs.cmu.edu/~./15251/) course staff during a grading session.
