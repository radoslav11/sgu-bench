import sys


def solve():
    # Read all integers from standard input at once for speed.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # Pointer into the data list.
    idx = 0

    # Read N, M, Q.
    n = data[idx]
    idx += 1
    m = data[idx]
    idx += 1
    q = data[idx]
    idx += 1

    # Store horizontal lines as pairs:
    # (Y, [representatives])
    lines = []

    # Read M horizontal lines.
    for _ in range(m):
        y = data[idx]
        idx += 1

        length = data[idx]
        idx += 1

        members = data[idx:idx + length]
        idx += length

        lines.append((y, members))

    # Read Q queries.
    queries = data[idx:idx + q]

    # Process lines from bottom to top.
    # Larger Y means lower/deeper, so sort by decreasing Y.
    lines.sort(key=lambda x: -x[0])

    # Total number of tree nodes:
    # leaves 1..n plus internal nodes n+1..n+m.
    total = n + m

    # DSU parent array.
    parent = list(range(n + 1))

    # node_of[root] stores the current tree node representing a DSU component.
    node_of = list(range(n + 1))

    # children[u] contains children of tree node u.
    children = [[] for _ in range(total + 1)]

    # Iterative DSU find with path compression.
    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    # Next internal node ID.
    next_id = n

    # Build the forest.
    for y, members in lines:
        # Create new internal node for this horizontal line.
        next_id += 1
        cur = next_id

        # Use the first member's component as the base component.
        base = find(members[0])

        # Connect all represented current subgroups as children of cur.
        for v in members:
            r = find(v)

            # The tree node representing this component becomes a child.
            children[cur].append(node_of[r])

            # Union this component into base.
            parent[r] = base

        # Get actual base root after possible path changes.
        base = find(base)

        # The merged component is now represented by cur.
        node_of[base] = cur

    # subtree_min[u] = smallest leaf ID inside subtree rooted at u.
    subtree_min = [0] * (total + 1)

    # Leaves contain only themselves.
    for i in range(1, n + 1):
        subtree_min[i] = i

    # Internal nodes are created after their children,
    # so increasing ID order works.
    INF = 10**30
    for u in range(n + 1, total + 1):
        best = INF

        # Minimum over all child subtrees.
        for c in children[u]:
            if subtree_min[c] < best:
                best = subtree_min[c]

        subtree_min[u] = best

    # Sort children of each internal node by their smallest contained leaf.
    for u in range(n + 1, total + 1):
        children[u].sort(key=lambda x: subtree_min[x])

    # Collect roots of the final forest.
    roots = []

    # Each DSU root is one connected component of the forest.
    for i in range(1, n + 1):
        if find(i) == i:
            roots.append(node_of[i])

    # Sort forest roots by smallest contained leaf.
    roots.sort(key=lambda x: subtree_min[x])

    # Produce final left-to-right leaf order by iterative DFS.
    order = []

    # Stack stores nodes to process.
    # Push roots in reverse order so popping processes leftmost first.
    stack = roots[::-1]

    while stack:
        u = stack.pop()

        # Leaf node.
        if u <= n:
            order.append(u)
        else:
            # Internal node.
            # Push children in reverse order so they are processed left-to-right.
            for c in reversed(children[u]):
                stack.append(c)

    # Answer queries.
    out = []

    for pos in queries:
        # Queries are 1-indexed.
        out.append(str(order[pos - 1]))

    # Print all answers.
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
