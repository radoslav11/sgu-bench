import sys


def build_suffix_array(s):
    """
    Builds suffix array for a list of integers s using the doubling algorithm.

    sa[i] = starting position of the i-th suffix in sorted order
    rank[pos] = position/rank of suffix starting at pos
    """

    n = len(s)

    # Initially suffixes are identified by their first value.
    sa = list(range(n))
    rank = s[:]

    # Temporary array for updated ranks.
    tmp = [0] * n

    k = 1

    while k < n:
        # Sort suffixes by pair:
        # (rank of first k values, rank of next k values)
        sa.sort(key=lambda i: (rank[i], rank[i + k] if i + k < n else -1))

        # First suffix in sorted order gets rank 0.
        tmp[sa[0]] = 0

        # Assign ranks to all suffixes.
        for i in range(1, n):
            prev = sa[i - 1]
            cur = sa[i]

            prev_key = (
                rank[prev],
                rank[prev + k] if prev + k < n else -1
            )

            cur_key = (
                rank[cur],
                rank[cur + k] if cur + k < n else -1
            )

            # Increase rank when key changes.
            tmp[cur] = tmp[prev] + (1 if prev_key != cur_key else 0)

        # Swap rank and tmp.
        rank, tmp = tmp, rank

        # If all suffixes have unique ranks, we are done.
        if rank[sa[-1]] == n - 1:
            break

        # Double compared prefix length.
        k <<= 1

    return sa, rank


def build_lcp(s, sa, rank):
    """
    Kasai algorithm.

    lcp[i] = LCP of suffixes sa[i] and sa[i - 1].
    lcp[0] = 0.
    """

    n = len(s)
    lcp = [0] * n

    h = 0

    for i in range(n):
        r = rank[i]

        if r == 0:
            h = 0
            continue

        # Previous suffix in suffix-array order.
        j = sa[r - 1]

        # Extend while equal.
        while i + h < n and j + h < n and s[i + h] == s[j + h]:
            h += 1

        lcp[r] = h

        # For next suffix, LCP decreases by at most one.
        if h:
            h -= 1

    return lcp


class SegmentTreeMin:
    """
    Segment tree for range minimum queries.

    It is used instead of a sparse table to keep the Python implementation
    memory-friendly.

    Query range is inclusive: query(l, r).
    """

    def __init__(self, arr):
        n = len(arr)

        self.size = 1
        while self.size < n:
            self.size <<= 1

        self.inf = 10 ** 18
        self.tree = [self.inf] * (2 * self.size)

        # Put original values at leaves.
        for i, x in enumerate(arr):
            self.tree[self.size + i] = x

        # Build internal nodes.
        for i in range(self.size - 1, 0, -1):
            self.tree[i] = min(self.tree[2 * i], self.tree[2 * i + 1])

    def query(self, l, r):
        """
        Returns min over inclusive interval [l, r].
        """

        l += self.size
        r += self.size

        ans = self.inf

        while l <= r:
            if l & 1:
                ans = min(ans, self.tree[l])
                l += 1

            if not (r & 1):
                ans = min(ans, self.tree[r])
                r -= 1

            l >>= 1
            r >>= 1

        return ans


def solve():
    data = list(map(int, sys.stdin.buffer.read().split()))

    n, m, k = data[0], data[1], data[2]
    a = data[3:3 + n]

    # Build doubled array so cyclic rotations become ordinary substrings.
    doubled = [a[i % n] for i in range(2 * n)]

    # Build suffix array and rank array.
    sa, rank = build_suffix_array(doubled)

    # Build LCP array.
    lcp = build_lcp(doubled, sa, rank)

    # RMQ over LCP array.
    rmq = SegmentTreeMin(lcp)

    def cyclic_lcp(p, q):
        """
        LCP of cyclic rotations starting at p and q, capped by n.
        """

        if p == q:
            return n

        rp = rank[p]
        rq = rank[q]

        if rp > rq:
            rp, rq = rq, rp

        # LCP of suffixes p and q is min lcp[rp + 1 ... rq].
        return min(n, rmq.query(rp + 1, rq))

    # groups[v] = all positions i where a[i] == v.
    groups = [[] for _ in range(m)]

    for i, x in enumerate(a):
        groups[x].append(i)

    # base[v] stores the original value at the k-th position of the best
    # rotation for the phase where original value v is the smallest.
    base = [0] * m

    # Wrap points caused by values that actually appear.
    wraps = []

    for v in range(m):
        if not groups[v]:
            continue

        # Value v becomes 0 at this shift.
        wraps.append((m - v) % m)

        # Find best cyclic rotation among starts with a[start] == v.
        best = groups[v][0]

        for c in groups[v][1:]:
            l = cyclic_lcp(c, best)

            # Equal rotations.
            if l >= n:
                continue

            # Compare first differing values after relabeling so v becomes 0.
            ca = (a[(c + l) % n] - v) % m
            cb = (a[(best + l) % n] - v) % m

            if ca < cb:
                best = c

        # k-th original value of that best rotation.
        base[v] = a[(best + k - 1) % n]

    # Sort phase starts.
    wraps.sort()

    ans = [0] * m
    w = len(wraps)

    # Fill answers phase by phase.
    for i in range(w):
        L = wraps[i]

        # Circular next wrap.
        R = wraps[i + 1] if i + 1 < w else wraps[0] + m

        # Original value that maps to 0 at L.
        v = (m - L) % m

        # Original k-th value in best rotation.
        bv = base[v]

        # For every t in this phase, answer is shifted by t modulo m.
        for t in range(L, R):
            tt = t % m
            ans[tt] = (bv + tt) % m

    sys.stdout.write("\n".join(map(str, ans)))


if __name__ == "__main__":
    solve()
