# o(n)-coloring 3-colorable graphs in O(n) time

### Summary

Suppose we're given a 3-colorable graph, and we want to give this graph a vertex
coloring. In general, we cannot hope to give an optimal coloring unless P = NP.
Instead we look for approximation algorithms that give a good coloring in
polynomial time. In 2014, Kawarabayashi and Thorup gave an algorithm that finds
an O(n^1.9996) coloring in polynomial time. This is the latest in a long
sequence of works that give O(n^c) colorings for steadily decreasing values of
c.

An interesting question that is useless both in theory and in practice is to
stipulate a stronger running time requirement. In particular, we wonder what the
best coloring achievable is using O(n) running time, where n is the number of
vertices in the graph. This means that if the graph is dense, we cannot even
afford to look at all the edges of the graph. Is it possible to color the graph
using o(n) colors?

In this project, we show that in the word RAM model, we can color a 3-colorable
graph with O(n / log log n) colors in O(n) time. See writeup/writeup.pdf for a
description of the algorithm.

### Algorithm

See writeup/writeup.pdf for details.

The high-level idea is as follows: we break the vertices into chunks of size k,
and for the subgraph induced by each chunk, we 3-color the subgraph by brute
force (using a different set of 3 colors for each subgraph). This gives a 3n/k
coloring of the graph. We set k to be just a little bigger than a constant. To
keep the runtime linear, we use some bit hackery to get some word-level
parallelism.

### Running the code

For fun (assuming a loose definition of "fun"),
this repository also contains some proof-of-concept code for the algorithm.

TODO

### Open questions

* This algorithm crucially relies on the word RAM model. Can we achieve an o(n)
  coloring without all the bit hackery?
* Is it possible to achieve a truly sublinear coloring, i.e. an O(n^(1 - c))
  coloring for some c > 0?
* What lower bounds can we prove assuming this O(n) running time restriction?

### Acknowledgments

This question was posed by some members of the Spring 2017
[15-251](http://www.cs.cmu.edu/~./15251/) course staff during a grading session.
