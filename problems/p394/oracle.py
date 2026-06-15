import sys
from bisect import bisect_left

# Fenwick Tree (Binary Indexed Tree) for prefix sums.
class Fenwick:
    def __init__(self, n: int):
        # Use 1-based indexing, size = n
        self.n = n
        self.bit = [0] * (n + 1)

    def add(self, i: int, delta: int) -> None:
        # Adds delta to index i
        while i <= self.n:
            self.bit[i] += delta
            i += i & -i

    def sum(self, i: int) -> int:
        # Returns prefix sum [1..i]
        s = 0
        while i > 0:
            s += self.bit[i]
            i -= i & -i
        return s


def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    k = int(next(it))

    x = [0] * n
    y = [0] * n
    w = [0] * n
    for i in range(n):
        x[i] = int(next(it))
        y[i] = int(next(it))
        w[i] = int(next(it))

    # Transform to (u, v) coordinates
    u = [0] * n
    v = [0] * n
    for i in range(n):
        u[i] = x[i] + y[i]
        v[i] = x[i] - y[i]

    # Collect all v-coordinates needed for compression:
    # v-w (range start), v+w+1 (end+1 for diff), v (query)
    coords = []
    for i in range(n):
        coords.append(v[i] - w[i])
        coords.append(v[i] + w[i] + 1)
        coords.append(v[i])
    coords = sorted(set(coords))

    # Compression function: value -> 1-based index
    def comp(val: int) -> int:
        return bisect_left(coords, val) + 1

    # Build events:
    # type 0 = update, type 1 = query (updates must come before queries at same u)
    events = []
    for i in range(n):
        events.append((u[i] - w[i], 0, i, 1))        # activate rectangle
        events.append((u[i] + w[i] + 1, 0, i, -1))   # deactivate (exclusive end)
        events.append((u[i], 1, i, 0))               # query point

    # Sort by u, then type (0 before 1)
    events.sort()

    fw = Fenwick(len(coords))
    cnt = [0] * n  # number of rectangles covering each point

    # Sweep line over u
    for ux, typ, idx, delta in events:
        if typ == 0:
            # Range add on v in [v-w, v+w] using diff:
            # add delta at L, add -delta at (R+1) which is (v+w+1)
            lo = comp(v[idx] - w[idx])
            hi = comp(v[idx] + w[idx] + 1)
            fw.add(lo, delta)
            fw.add(hi, -delta)
        else:
            # Point query at v[idx]
            cnt[idx] = fw.sum(comp(v[idx]))

    # Hut i is closable if at least K OTHER huts cover it => cnt[i]-1 >= K
    res = [str(i + 1) for i in range(n) if cnt[i] - 1 >= k]

    # Output
    out = []
    out.append(str(len(res)))
    out.append(" ".join(res))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
