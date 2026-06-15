import sys

import numpy as np


def solve() -> None:
    lines = sys.stdin.buffer.read().splitlines()

    # Active segments in two parallel numpy buffers of capacity cap; n is the
    # number of live entries. '+' counts containment with one vectorized
    # comparison, '-' swap-removes the first matching (L, R) pair.
    cap = 4096
    Ls = np.empty(cap, dtype=np.int64)
    Rs = np.empty(cap, dtype=np.int64)
    n = 0

    out_lines = []

    for line in lines:
        if not line:
            continue
        t, ls, rs = line.split()
        L = int(ls)
        R = int(rs)

        if t == b"+":
            cnt = int(np.count_nonzero((Ls[:n] >= L) & (Rs[:n] <= R)))
            out_lines.append(str(cnt))
            if n == cap:
                cap *= 2
                Ls = np.resize(Ls, cap)
                Rs = np.resize(Rs, cap)
            Ls[n] = L
            Rs[n] = R
            n += 1
        else:
            idx = np.flatnonzero((Ls[:n] == L) & (Rs[:n] == R))
            if idx.size:
                i = int(idx[0])
                Ls[i] = Ls[n - 1]
                Rs[i] = Rs[n - 1]
                n -= 1

    if out_lines:
        sys.stdout.write("\n".join(out_lines) + "\n")


if __name__ == "__main__":
    solve()
