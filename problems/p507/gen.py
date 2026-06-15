"""Tests for p507 (Treediff): for every internal node output the minimum
absolute difference between leaf values in its subtree. n total nodes,
m leaves; internal nodes are 1..n-m, leaves are n-m+1..n. Leaf values are
in [-1e9, 1e9].

The reference DFS is recursive, so tests keep the tree shallow (bounded
depth). Seed 1 is the smallest tree. Seed 2 builds a balanced internal
tree with m maximal and all-distinct leaf values: the heaviest small-to-
large merging case. Seed 3 maximises the number of internal nodes with a
single leaf each (a long "caterpillar" of bounded depth). Later seeds add
duplicate values (answer 0), tiny value ranges (frequent ties), star and
bushy shapes.
"""

import random
import sys

NMAX = 100000
VMAX = 10**9
INF = 2147483647


def build(internal_parent, leaf_parent, leaf_values):
    """internal_parent[i] is parent of internal node i (2..I), 1-based.
    leaf_parent[k] is parent of leaf k (I+1..n). leaf_values matches leaves.
    Emits the input in the required format."""
    out = []
    I = len(internal_parent) + 1  # number of internal nodes
    m = len(leaf_values)
    n = I + m
    out.append(f"{n} {m}")
    # parents of nodes 2..n in order
    for i in range(2, I + 1):
        out.append(str(internal_parent[i]))
    for k in range(m):
        out.append(str(leaf_parent[k]))
    for v in leaf_values:
        out.append(str(v))
    sys.stdout.write("\n".join(out) + "\n")


def random_internal_tree(rng, I, max_back):
    """Tree on internal nodes 1..I; node i's parent is chosen from the
    last `max_back` earlier nodes to keep the tree both random and shallow.
    Returns parent dict for 2..I."""
    parent = {}
    for i in range(2, I + 1):
        lo = max(1, i - max_back)
        parent[i] = rng.randint(lo, i - 1)
    return parent


def attach_leaves(rng, I, m, parent):
    """Return leaf_parent list of length m so that every internal node ends
    up with at least one child. Internal nodes that have no internal child
    are guaranteed at least one leaf; the rest of the leaves go randomly."""
    has_child = [False] * (I + 1)
    for i in range(2, I + 1):
        has_child[parent[i]] = True
    childless = [i for i in range(1, I + 1) if not has_child[i]]
    if len(childless) > m:
        return None  # caller must ensure m is large enough
    leaf_parent = list(childless)
    for _ in range(m - len(childless)):
        leaf_parent.append(rng.randint(1, I))
    rng.shuffle(leaf_parent)
    return leaf_parent


def emit_general(rng, I, m, max_back, values):
    parent = random_internal_tree(rng, I, max_back)
    leaf_parent = attach_leaves(rng, I, m, parent)
    if leaf_parent is None:
        # Not enough leaves; fall back to a flat tree under the root.
        parent = {i: 1 for i in range(2, I + 1)}
        leaf_parent = attach_leaves(rng, I, m, parent)
    build(parent, leaf_parent, values)


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        # Smallest tree: root with one leaf.
        build({}, [1], [rng.randint(-VMAX, VMAX)])
        return

    if seed == 2:
        # Balanced internal tree (parent = i//2): depth ~log n. m maximal,
        # all leaf values distinct -> heaviest merging, non-trivial diffs.
        I = NMAX // 2
        m = NMAX - I
        parent = {i: i // 2 for i in range(2, I + 1)}
        leaf_parent = attach_leaves(rng, I, m, parent)
        vals = rng.sample(range(-VMAX, VMAX), m)
        build(parent, leaf_parent, vals)
        return

    if seed == 3:
        # Caterpillar of bounded depth: a shallow random internal tree with
        # exactly one leaf per internal node where possible.
        I = NMAX // 2
        m = NMAX - I
        vals = rng.sample(range(-VMAX, VMAX), m)
        emit_general(rng, I, m, max_back=200, values=vals)
        return

    if seed == 4:
        # Many leaves, few internal nodes (a wide, shallow tree). Tight
        # value range so duplicate values appear and many answers are 0.
        I = 2000
        m = NMAX - I
        vals = [rng.randint(0, 1000) for _ in range(m)]
        emit_general(rng, I, m, max_back=I, values=vals)
        return

    if seed == 5:
        # Star: root is the only internal node, all leaves hang off it.
        I = 1
        m = NMAX - I
        vals = rng.sample(range(-VMAX, VMAX), m)
        build({}, [1] * m, vals)
        return

    if seed == 6:
        # Balanced tree but values from a small range -> frequent ties,
        # answer 0 propagating up many subtrees.
        I = NMAX // 2
        m = NMAX - I
        parent = {i: i // 2 for i in range(2, I + 1)}
        leaf_parent = attach_leaves(rng, I, m, parent)
        vals = [rng.randint(-50, 50) for _ in range(m)]
        build(parent, leaf_parent, vals)
        return

    if seed == 7:
        # Two big subtrees under the root, each balanced; forces a large
        # final merge at the root.
        I = NMAX // 2
        m = NMAX - I
        parent = {}
        for i in range(2, I + 1):
            if i == 2 or i == 3:
                parent[i] = 1
            else:
                parent[i] = max(2, i // 2)
        leaf_parent = attach_leaves(rng, I, m, parent)
        if leaf_parent is None:
            emit_general(rng, I, m, max_back=I, values=rng.sample(range(-VMAX, VMAX), m))
            return
        vals = rng.sample(range(-VMAX, VMAX), m)
        build(parent, leaf_parent, vals)
        return

    if seed == 8:
        # All leaf values equal -> every internal node answer is 0.
        I = NMAX // 3
        m = NMAX - I
        emit_general(rng, I, m, max_back=300, values=[7] * m)
        return

    # General randomized mid-size tests.
    I = rng.randint(2, NMAX // 2)
    m = rng.randint(max(1, I), NMAX - I)
    rng_range = rng.choice([VMAX, 1000, 5, 100000])
    if rng_range == VMAX and m <= 2 * VMAX:
        vals = rng.sample(range(-VMAX, VMAX), m)
    else:
        vals = [rng.randint(-rng_range, rng_range) for _ in range(m)]
    emit_general(rng, I, m, max_back=rng.randint(1, I), values=vals)


if __name__ == "__main__":
    main()
