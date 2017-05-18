import random
import string
import numpy as np
# Generates a tripartite graph.

# parts[i] is the number of vertices of the i-th set in a partition of the
# graph, where no edges go between two vertices in the same set.
parts = [5, 5, 5]
# probability of having an edge between any two vertices that belong to
# different sets in the partition
p = 0.5

partial_sums = [0 for _ in xrange(len(parts) + 1)]
for i in xrange(1, len(partial_sums)):
    partial_sums[i] = partial_sums[i - 1] + parts[i - 1]
n = partial_sums[-1]

# add edges
edges = []
for p1 in xrange(len(parts)):
    voffset1 = partial_sums[p1]
    for p2 in xrange(p1 + 1, len(parts)):
        voffset2 = partial_sums[p2]
        for v1 in xrange(parts[p1]):
            for v2 in xrange(parts[p2]):
                if random.uniform(0, 1) >= p:
                    edges.append((voffset1 + v1, voffset2 + v2))

# permute vertex names and permute edge ordering
vperm = np.random.permutation(n)
edges = map(lambda (a, b): 
              (vperm[a], vperm[b]) if random.randint(0, 1) == 0 else (vperm[b], vperm[a]), 
            edges)
edges = np.random.permutation(edges)

random_string = ''.join(random.choice(string.lowercase) for i in range(6))
with open('generated-' + random_string + '.txt', 'w') as f:
    f.write('{} {}\n'.format(n, len(edges)))
    for (u, v) in edges:
        f.write('{} {} '.format(u, v))
