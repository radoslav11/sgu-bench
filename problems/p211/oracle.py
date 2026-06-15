import sys
from bisect import bisect_left, bisect_right
from sortedcontainers import SortedSet

def solve():
    data = list(map(int, sys.stdin.read().strip().split()))
    if not data:
        return
    it = iter(data)
    n = next(it)
    m = next(it)
    queries = [next(it) for _ in range(m)]

    # reg[i] is digit at position i, digits are always in {0,1,2}
    # size n+2 to safely use positions up to n+1 (sentinel at n+1)
    reg = [0] * (n + 2)

    # Keep positions for each digit value in a sorted set.
    # Using SortedSet (from sortedcontainers) provides:
    # - O(log N) add/remove
    # - O(log N) bisect-based successor/predecessor queries
    pos = [SortedSet() for _ in range(3)]

    # Initially reg[0..n] are 0 (include n as buffer), insert them.
    for i in range(0, n + 1):
        pos[0].add(i)

    # Sentinel "2" at n+1 so a 'next_two' always exists.
    pos[2].add(n + 1)
    reg[n + 1] = 2  # keep internal consistency (never printed)

    out_lines = []

    def apply(changes, idx, delta):
        """Update reg[idx] += delta, maintain pos sets, and record the change."""
        old = reg[idx]
        pos[old].remove(idx)          # remove from old value's set
        reg[idx] = old + delta        # update digit
        pos[reg[idx]].add(idx)        # add to new value's set
        changes.append((idx, reg[idx]))

    for p in queries:
        # next_two: smallest index > p with digit 2
        j = pos[2].bisect_right(p)
        next_two = pos[2][j]

        # prev_zero: largest index < next_two with digit 0
        k = pos[0].bisect_left(next_two) - 1
        prev_zero = pos[0][k]

        changes = []

        # Case 1
        if next_two <= n and p >= prev_zero:
            apply(changes, p, +1)
            apply(changes, next_two, -2)
            apply(changes, next_two + 1, +1)

        # Case 2
        elif reg[p] != 0:
            apply(changes, p, -1)
            apply(changes, p + 1, +1)

        # Case 3
        else:
            apply(changes, p, +1)

        # Format output: K followed by K pairs
        parts = [str(len(changes))]
        for idx, val in changes:
            parts.append(str(idx))
            parts.append(str(val))
        out_lines.append(" ".join(parts))

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
