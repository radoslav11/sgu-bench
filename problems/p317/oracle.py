import sys
import bisect
from math import inf

# We implement:
# - A dynamic CHT for minimum queries using the same multiset idea,
#   but in Python we’ll use a "Li Chao segment tree" to keep it simpler and robust.
#
# Why Li Chao?
# - Supports add line and query at x in O(log Xrange).
# - Works well with doubles too.
# Here X coordinates are up to 1e8, but we only query at stable positions,
# so we can compress x-values per segment-tree node (offline Li Chao) or do
# coordinate-compressed Li Chao globally.
#
# However, we also need "range add line" and "point query", so we keep the
# segment tree over indices, and each node stores a Li Chao tree operating on
# x-values (stable positions) globally (since queries use x=stables[pos].x).
#
# To keep memory acceptable in Python, we implement Li Chao as a dict-based
# implicit tree, and only create nodes when needed.

class LiChaoMin:
    """Li Chao segment tree for minimum of lines on a fixed x-domain."""
    __slots__ = ("xs", "line_k", "line_m", "left", "right")

    def __init__(self, xs):
        # xs: sorted list of x-coordinates where we will query (discrete domain).
        self.xs = xs
        self.line_k = None
        self.line_m = None
        self.left = None
        self.right = None

    @staticmethod
    def f(k, m, x):
        return k * x + m

    def add_line(self, k, m, l=0, r=None):
        if r is None:
            r = len(self.xs) - 1

        if self.line_k is None:
            self.line_k, self.line_m = k, m
            return

        mid = (l + r) // 2
        x_l = self.xs[l]
        x_m = self.xs[mid]
        x_r = self.xs[r]

        cur_k, cur_m = self.line_k, self.line_m

        # At midpoint, decide which line is better (smaller).
        if self.f(k, m, x_m) < self.f(cur_k, cur_m, x_m):
            # Swap: keep better line in this node
            self.line_k, self.line_m, k, m = k, m, cur_k, cur_m
            cur_k, cur_m = self.line_k, self.line_m

        if l == r:
            return

        # Now cur line is best at mid. The other line may be better on left or right.
        if self.f(k, m, x_l) < self.f(cur_k, cur_m, x_l):
            if self.left is None:
                self.left = LiChaoMin(self.xs)
            self.left.add_line(k, m, l, mid)
        elif self.f(k, m, x_r) < self.f(cur_k, cur_m, x_r):
            if self.right is None:
                self.right = LiChaoMin(self.xs)
            self.right.add_line(k, m, mid + 1, r)

    def query(self, x, l=0, r=None):
        if r is None:
            r = len(self.xs) - 1

        res = inf
        if self.line_k is not None:
            res = self.f(self.line_k, self.line_m, x)

        if l == r:
            return res

        mid = (l + r) // 2
        if x <= self.xs[mid]:
            if self.left is None:
                return res
            return min(res, self.left.query(x, l, mid))
        else:
            if self.right is None:
                return res
            return min(res, self.right.query(x, mid + 1, r))


class SegTree:
    """Segment tree over stable indices; each node stores Li Chao for min lines."""
    def __init__(self, xs):
        self.n = len(xs)
        self.xs = xs
        self.tree = [None] * (4 * self.n)

    def _ensure(self, idx):
        if self.tree[idx] is None:
            self.tree[idx] = LiChaoMin(self.xs)

    def update(self, ql, qr, k, m, idx=1, lo=0, hi=None):
        if hi is None:
            hi = self.n - 1
        if ql > hi or qr < lo:
            return
        if ql <= lo and hi <= qr:
            self._ensure(idx)
            self.tree[idx].add_line(k, m)
            return
        mid = (lo + hi) // 2
        self.update(ql, qr, k, m, idx * 2, lo, mid)
        self.update(ql, qr, k, m, idx * 2 + 1, mid + 1, hi)

    def query(self, pos, x, idx=1, lo=0, hi=None):
        if hi is None:
            hi = self.n - 1
        res = inf
        if self.tree[idx] is not None:
            res = self.tree[idx].query(x)
        if lo == hi:
            return res
        mid = (lo + hi) // 2
        if pos <= mid:
            return min(res, self.query(pos, x, idx * 2, lo, mid))
        else:
            return min(res, self.query(pos, x, idx * 2 + 1, mid + 1, hi))


def solve():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    B = int(next(it))
    N = int(next(it))

    stables = []
    for _ in range(N):
        x = int(next(it))
        m = int(next(it))
        horses = []
        for __ in range(m):
            v = int(next(it))
            d = int(next(it))
            horses.append((d, v))  # store as (distance, speed)
        stables.append((x, horses))

    # sort by coordinate and drop stables beyond B
    stables.sort(key=lambda t: t[0])
    while stables and stables[-1][0] > B:
        stables.pop()

    if not stables or stables[0][0] != 0:
        sys.stdout.write("-1\n")
        return

    xs = [x for x, _ in stables]
    n = len(stables)

    seg = SegTree(xs)
    dp = [inf] * n
    ans = inf

    for i, (xi, horses) in enumerate(stables):
        if xi == 0:
            dp[i] = 0.0

        if i > 0:
            best = seg.query(i, float(xi))
            if best < dp[i]:
                dp[i] = best

        if dp[i] == inf:
            continue

        for d, v in horses:
            reach = xi + d

            # try finishing directly to B
            if reach >= B:
                ans = min(ans, dp[i] + (B - xi) / v)

            # compute r = last index with x <= reach
            r = bisect.bisect_right(xs, reach) - 1
            if r > i:
                k = 1.0 / v
                m = dp[i] - xi / v
                seg.update(i + 1, r, k, m)

    if ans == inf:
        sys.stdout.write("-1\n")
    else:
        sys.stdout.write(f"{ans:.8f}\n")


if __name__ == "__main__":
    solve()
