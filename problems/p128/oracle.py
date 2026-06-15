import sys
from collections import defaultdict

# Fast IO
input_data = sys.stdin.read().strip().split()
it = iter(input_data)

def read_int():
    return int(next(it))

def main():
    # Read number of points
    try:
        n = read_int()
    except StopIteration:
        return

    points = []
    for _ in range(n):
        x = read_int()
        y = read_int()
        points.append((x, y))

    # If odd number of points, impossible to form a closed rectilinear polygon
    if n % 2 == 1:
        print(0)
        return

    # Group points by same x and same y
    # For each x: list of (y, index)
    pnts_on_x = defaultdict(list)
    # For each y: list of (x, index)
    pnts_on_y = defaultdict(list)

    for idx, (x, y) in enumerate(points):
        pnts_on_x[x].append((y, idx))
        pnts_on_y[y].append((x, idx))

    # DSU (Disjoint Set Union) implementation
    parent = list(range(n))   # parent[i] = parent of i
    size = [1] * n            # size[i] = size of component with root i

    def find(u):
        # Path compression
        while parent[u] != u:
            parent[u] = parent[parent[u]]
            u = parent[u]
        return u

    def unite(a, b):
        # Union by size
        ra = find(a)
        rb = find(b)
        if ra == rb:
            return
        if size[ra] > size[rb]:
            ra, rb = rb, ra
        parent[ra] = rb
        size[rb] += size[ra]

    # List of edges: ((x1,y1), (x2,y2))
    edges = []
    total_length = 0

    # Process vertical segments on each x
    for x, vec in pnts_on_x.items():
        # vec is list of (y, index)
        vec.sort(key=lambda p: p[0])  # sort by y

        # Must have even count of points on this x
        if len(vec) % 2 == 1:
            print(0)
            return

        # Pair consecutive entries
        for i in range(0, len(vec), 2):
            y1, idx1 = vec[i]
            y2, idx2 = vec[i + 1]

            # Points with exactly same (x,y) would imply zero-length segment: invalid
            if y1 == y2:
                print(0)
                return

            # Add vertical edge
            edges.append(((x, y1), (x, y2)))
            total_length += abs(y2 - y1)
            # Union indices
            unite(idx1, idx2)

    # Process horizontal segments on each y
    for y, vec in pnts_on_y.items():
        # vec is list of (x, index)
        vec.sort(key=lambda p: p[0])  # sort by x

        # Must have even count of points on this y
        if len(vec) % 2 == 1:
            print(0)
            return

        for i in range(0, len(vec), 2):
            x1, idx1 = vec[i]
            x2, idx2 = vec[i + 1]

            # Zero-length horizontal segment is invalid
            if x1 == x2:
                print(0)
                return

            # Add horizontal edge
            edges.append(((x1, y), (x2, y)))
            total_length += abs(x2 - x1)
            # Union indices
            unite(idx1, idx2)

    # Check that all vertices are in one connected component
    root0 = find(0)
    for i in range(1, n):
        if find(i) != root0:
            print(0)
            return

    # Helper function to check if two axis-aligned segments intersect improperly
    def segments_intersect(a1, a2, b1, b2):
        x1a, y1a = a1
        x2a, y2a = a2
        x1b, y1b = b1
        x2b, y2b = b2

        # Helper: check strict 1D overlap of segments [a1,a2] and [b1,b2]
        def overlap_1d(a1, a2, b1, b2):
            # open interval overlap
            return max(min(a1, a2), min(b1, b2)) < min(max(a1, a2), max(b1, b2))

        # Detect whether segment is vertical or horizontal
        a_vertical = (x1a == x2a)
        b_vertical = (x1b == x2b)
        a_horizontal = (y1a == y2a)
        b_horizontal = (y1b == y2b)

        # Case 1: both vertical
        if a_vertical and b_vertical:
            # same x and overlapping y-ranges => invalid
            if x1a == x1b and overlap_1d(y1a, y2a, y1b, y2b):
                return True
            return False

        # Case 2: both horizontal
        if a_horizontal and b_horizontal:
            # same y and overlapping x-ranges => invalid
            if y1a == y1b and overlap_1d(x1a, x2a, x1b, x2b):
                return True
            return False

        # Normalize so that 'a' is vertical and 'b' is horizontal if possible
        if a_horizontal and b_vertical:
            # swap segments
            (x1a, y1a, x2a, y2a, x1b, y1b, x2b, y2b) = (x1b, y1b, x2b, y2b,
                                                        x1a, y1a, x2a, y2a)
            a_vertical, b_vertical = True, False
            a_horizontal, b_horizontal = False, True

        # Case 3: a is vertical, b is horizontal
        if a_vertical and b_horizontal:
            # vertical at x = x1a between y1a,y2a; horizontal at y = y1b between x1b,x2b
            # check proper intersection in interiors (strict inequalities)
            if (min(y1a, y2a) < y1b < max(y1a, y2a) and
                min(x1b, x2b) < x1a < max(x1b, x2b)):
                return True
            return False

        # Other orientation combos shouldn't occur for valid axis-aligned segments
        return False

    m = len(edges)

    # Check all pairs of edges; O(m^2)
    for i in range(m):
        a1, a2 = edges[i]
        for j in range(i + 1, m):
            b1, b2 = edges[j]
            if segments_intersect(a1, a2, b1, b2):
                print(0)
                return

    # All checks passed: output total length
    print(total_length)


if __name__ == "__main__":
    main()
