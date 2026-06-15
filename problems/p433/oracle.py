import sys
from math import gcd


def solve():
    # Read all integers from input.
    data = list(map(int, sys.stdin.read().split()))

    # First three integers are N, M, K.
    n, m, k = data[0], data[1], data[2]

    # Next K integers are allowed brick lengths.
    lengths = data[3:3 + k]

    # We only care about lengths up to the larger rectangle dimension.
    dim = max(n, m)

    # rep[x] tells whether length x can be represented as a sum of brick lengths.
    rep = [False] * (dim + 1)

    # par[x] stores one brick length used in a representation of x.
    par = [-1] * (dim + 1)

    # Zero length is representable using no bricks.
    rep[0] = True

    # Unbounded knapsack / coin-change DP.
    for s in range(1, dim + 1):
        for length in lengths:
            if length <= s and rep[s - length]:
                rep[s] = True
                par[s] = length
                break

    # Compute gcd of all usable brick lengths.
    g = 0
    for length in lengths:
        if length <= dim:
            g = gcd(g, length)

    # canon[x] is a compressed representative of run length x.
    canon = [0] * (dim + 1)

    if g == 0:
        # No brick can fit at all, so no periodic compression.
        for s in range(dim + 1):
            canon[s] = s
    else:
        # Find where representability stabilizes to "s is divisible by gcd".
        pre = 0
        for s in range(dim + 1):
            if rep[s] != (s % g == 0):
                pre = s + 1

        # Before pre, keep exact lengths.
        # From pre onward, only residue modulo gcd matters.
        for s in range(dim + 1):
            if s < pre:
                canon[s] = s
            else:
                canon[s] = pre + (s - pre) % g

    # Work internally with the smaller dimension as height.
    transposed = n > m
    h = min(n, m)
    w = max(n, m)

    # horiz[r][c] = 1 means horizontal brick orientation,
    # horiz[r][c] = 0 means vertical brick orientation.
    horiz = [[0] * w for _ in range(h)]

    possible = True

    if rep[w]:
        # If the width is representable, make every row horizontal.
        for r in range(h):
            for c in range(w):
                horiz[r][c] = 1

    elif not rep[h]:
        # Hard case: neither width nor height is directly representable.

        valid_cols = []

        # Enumerate all column masks of height h.
        for mask in range(1 << h):
            ok = True
            r = 0

            while r < h:
                # Bit 0 means vertical orientation.
                if ((mask >> r) & 1) == 0:
                    r2 = r

                    # Find maximal vertical zero-run.
                    while r2 < h and ((mask >> r2) & 1) == 0:
                        r2 += 1

                    # Its length must be representable.
                    if not rep[r2 - r]:
                        ok = False
                        break

                    r = r2
                else:
                    r += 1

            if ok:
                valid_cols.append(mask)

        # Each DP layer is a list of states.
        # A state is (runs, parent_index, mask_used).
        layers = [[] for _ in range(w + 1)]

        # seen[c] maps canonical state keys to indices in layers[c].
        seen = [dict() for _ in range(w + 1)]

        # Initial state: no open horizontal runs.
        start_runs = tuple([0] * h)
        layers[0].append((start_runs, -1, -1))
        seen[0][start_runs] = 0

        def key_of(runs):
            # Compress each run length using canon[].
            return tuple(canon[x] for x in runs)

        # Process all columns left to right.
        for c in range(w):
            for i, (runs, parent, old_mask) in enumerate(layers[c]):
                # Try every vertically valid column mask.
                for mask in valid_cols:
                    nxt = [0] * h
                    ok = True

                    for r in range(h):
                        if (mask >> r) & 1:
                            # Horizontal cell extends the run.
                            nxt[r] = runs[r] + 1
                        else:
                            # Vertical cell closes the horizontal run.
                            if runs[r] > 0 and not rep[runs[r]]:
                                ok = False
                                break

                            nxt[r] = 0

                    if not ok:
                        continue

                    nxt_tuple = tuple(nxt)
                    key = key_of(nxt_tuple)

                    # Store only one representative of equivalent states.
                    if key not in seen[c + 1]:
                        seen[c + 1][key] = len(layers[c + 1])
                        layers[c + 1].append((nxt_tuple, i, mask))

        # Find accepting final state.
        final_idx = -1

        for i, (runs, parent, mask) in enumerate(layers[w]):
            ok = True

            for r in range(h):
                if runs[r] > 0 and not rep[runs[r]]:
                    ok = False
                    break

            if ok:
                final_idx = i
                break

        if final_idx == -1:
            possible = False
        else:
            # Reconstruct column masks.
            idx = final_idx

            for c in range(w, 0, -1):
                runs, parent, mask = layers[c][idx]

                for r in range(h):
                    horiz[r][c - 1] = (mask >> r) & 1

                idx = parent

    # Else:
    # rep[w] is false and rep[h] is true.
    # The default all-zero matrix means all bricks are vertical.

    if not possible:
        print("NO")
        return

    # bid[r][c] stores the integer brick ID covering internal cell (r, c).
    bid = [[-1] * w for _ in range(h)]

    next_id = 0

    # Process horizontal runs.
    for r in range(h):
        c = 0

        while c < w:
            if horiz[r][c] == 1:
                c2 = c

                # Find maximal horizontal run of 1s.
                while c2 < w and horiz[r][c2] == 1:
                    c2 += 1

                pos = c
                rem = c2 - c

                # Split run into bricks using par[].
                while rem > 0:
                    length = par[rem]

                    for t in range(length):
                        bid[r][pos + t] = next_id

                    next_id += 1
                    pos += length
                    rem -= length

                c = c2
            else:
                c += 1

    # Process vertical runs.
    for c in range(w):
        r = 0

        while r < h:
            if horiz[r][c] == 0:
                r2 = r

                # Find maximal vertical run of 0s.
                while r2 < h and horiz[r2][c] == 0:
                    r2 += 1

                pos = r
                rem = r2 - r

                # Split run into bricks using par[].
                while rem > 0:
                    length = par[rem]

                    for t in range(length):
                        bid[pos + t][c] = next_id

                    next_id += 1
                    pos += length
                    rem -= length

                r = r2
            else:
                r += 1

    # Total number of bricks.
    b = next_id

    # Build adjacency graph between bricks.
    nbr = [set() for _ in range(b)]

    for r in range(h):
        for c in range(w):
            # Right neighbor.
            if c + 1 < w and bid[r][c] != bid[r][c + 1]:
                a = bid[r][c]
                bb = bid[r][c + 1]
                nbr[a].add(bb)
                nbr[bb].add(a)

            # Bottom neighbor.
            if r + 1 < h and bid[r][c] != bid[r + 1][c]:
                a = bid[r][c]
                bb = bid[r + 1][c]
                nbr[a].add(bb)
                nbr[bb].add(a)

    # Smallest-last ordering for greedy coloring.
    deg = [len(nbr[i]) for i in range(b)]
    removed = [False] * b
    order = []

    for _ in range(b):
        u = -1

        # Pick remaining vertex with minimum degree.
        for i in range(b):
            if not removed[i] and (u == -1 or deg[i] < deg[u]):
                u = i

        removed[u] = True
        order.append(u)

        # Update degrees.
        for v in nbr[u]:
            if not removed[v]:
                deg[v] -= 1

    # Greedy color in reverse removal order.
    color = [-1] * b

    for u in reversed(order):
        used = set()

        for v in nbr[u]:
            if color[v] != -1:
                used.add(color[v])

        col = 0
        while col in used:
            col += 1

        color[u] = col

    # Build output grid in original orientation.
    out = [['?'] * m for _ in range(n)]

    for i in range(n):
        for j in range(m):
            if transposed:
                brick_id = bid[j][i]
            else:
                brick_id = bid[i][j]

            out[i][j] = chr(ord('a') + color[brick_id])

    print("YES")
    for row in out:
        print(''.join(row))


if __name__ == "__main__":
    solve()
