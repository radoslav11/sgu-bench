import sys

# We will implement an iterative segment tree for range-max over pairs (dp, index).
# Python tuples compare lexicographically, so max() on tuples works directly.

INT_MIN = -10**18  # sufficiently small sentinel

MAXV = 10**6 + 42  # maximum color domain in segment tree


class SegmentTree:
    """
    Segment tree for range maximum over pairs (dp, index) on domain [0..n-1].
    merge: max of two tuples.
    identity: (INT_MIN, -1)
    """

    def __init__(self, n):
        # n is number of leaves (here n = MAXV+1).
        self.n = n
        # Find power-of-two base size >= n.
        size = 1
        while size < n:
            size <<= 1
        self.size = size
        # Allocate tree. Use list of tuples; initialize to identity.
        self.tr = [(INT_MIN, -1)] * (2 * size)

    def update(self, pos, val):
        """
        Point update at position pos with value val.
        We set tr[pos] = val (not taking max here; caller should do that).
        """
        i = pos + self.size
        self.tr[i] = val
        i >>= 1
        # Recompute all parents.
        while i > 0:
            left = self.tr[2 * i]
            right = self.tr[2 * i + 1]
            # Node value is maximum of children.
            self.tr[i] = left if left >= right else right
            i >>= 1

    def get_pos(self, pos):
        """Return the value stored at leaf pos (0-based)."""
        return self.tr[pos + self.size]

    def query(self, l, r):
        """
        Range max on [l, r] inclusive, 0-based.
        """
        if l > r:
            return (INT_MIN, -1)
        n = self.size
        # Convert [l, r] to [l, r+1) on leaf indices.
        l += n
        r += n + 1
        resl = (INT_MIN, -1)
        resr = (INT_MIN, -1)
        while l < r:
            if l & 1:
                # Take current segment at l
                if self.tr[l] > resl:
                    resl = self.tr[l]
                l += 1
            if r & 1:
                r -= 1
                if self.tr[r] > resr:
                    resr = self.tr[r]
            l >>= 1
            r >>= 1
        # Combine left and right result.
        return resl if resl >= resr else resr


def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    c = [int(next(it)) for _ in range(n)]

    # Build segment tree on color domain [0..MAXV].
    seg = SegmentTree(MAXV + 1)

    dp = [0] * n       # dp[i] = best subsequence length ending at i
    parent = [-1] * n  # parent[i] = previous index in subsequence

    # Initialize first element.
    dp[0] = 1
    parent[0] = -1
    # At color c[0], store (dp[0], index=0)
    old = seg.get_pos(c[0])
    # Keep maximum between old and new.
    best_pair = old if old >= (dp[0], 0) else (dp[0], 0)
    seg.update(c[0], best_pair)

    for i in range(1, n):
        col = c[i]
        # best = (best_dp, index)
        best_dp, best_idx = 0, -1

        # Query [0, col-2] if col >= 2.
        if col >= 2:
            cand = seg.query(0, col - 2)
            if cand[0] > best_dp:
                best_dp, best_idx = cand

        # Query [col, col] (same color).
        cand = seg.query(col, col)
        if cand[0] > best_dp:
            best_dp, best_idx = cand

        # Query [col+2, MAXV] if applicable.
        if col + 2 <= MAXV:
            cand = seg.query(col + 2, MAXV)
            if cand[0] > best_dp:
                best_dp, best_idx = cand

        # Compute dp[i] and parent[i].
        dp[i] = best_dp + 1
        parent[i] = best_idx

        # Update segment tree at this color with (dp[i], i).
        old = seg.get_pos(col)
        new_pair = (dp[i], i)
        best_pair = old if old >= new_pair else new_pair
        seg.update(col, best_pair)

    # Find global maximum dp and its index.
    max_len = max(dp)
    end_pos = max(range(n), key=lambda i: dp[i])

    # Reconstruct path of indices.
    path = []
    cur = end_pos
    while cur != -1:
        path.append(cur)
        cur = parent[cur]
    path.reverse()

    # Minimum deletions = n - max_len.
    out_lines = []
    out_lines.append(str(n - max_len))
    # Build the resulting sequence string.
    seq = ' '.join(str(c[i]) for i in path)
    out_lines.append(seq)
    sys.stdout.write('\n'.join(out_lines) + '\n')


if __name__ == "__main__":
    solve()
