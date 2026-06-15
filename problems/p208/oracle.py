import sys

# ---------- Big integer ----------
# Python already has arbitrary-precision integers, so we do not need a custom big-int.
# That makes the implementation much shorter.

def count_cycles(n, m, perm):
    """
    Count cycles in the permutation of an n x m grid defined by perm(r, c).
    perm returns (nr, nc).
    """
    vis = [[False] * m for _ in range(n)]
    cycles = 0

    for r in range(n):
        for c in range(m):
            if not vis[r][c]:
                cycles += 1
                cr, cc = r, c
                # Follow the permutation until the cycle closes
                while not vis[cr][cc]:
                    vis[cr][cc] = True
                    cr, cc = perm(cr, cc)

    return cycles


def solve():
    data = sys.stdin.read().strip().split()
    n, m = map(int, data[:2])

    nm = n * m

    # Precompute powers of two: pw2[k] = 2^k for k=0..nm
    # (This is optional in Python since pow(2,k) is fast, but mirrors the C++ logic.)
    pw2 = [1] * (nm + 1)
    for i in range(1, nm + 1):
        pw2[i] = pw2[i - 1] * 2

    # Group size depending on whether square
    group_size = 4 * n * m if n == m else 2 * n * m

    total = 0  # sum of fixed colorings over all group elements

    # 1) Translations and 180-degree rotations (both exist for any n,m)
    for a in range(n):
        for b in range(m):
            # Translation: (r,c) -> (r+a, c+b)
            k = count_cycles(n, m, lambda r, c: ((r + a) % n, (c + b) % m))
            total += pw2[k]

            # 180-degree rotation + translation: (r,c) -> (-r+a, -c+b)
            # Using modular form: (-r) mod n == (n-r) mod n (for r in 0..n-1)
            k = count_cycles(n, m, lambda r, c: ((n - r + a) % n, (m - c + b) % m))
            total += pw2[k]

    # 2) If square, add 90 and 270 degree rotations + translations
    if n == m:
        for a in range(n):
            for b in range(n):
                # 90-degree: (r,c) -> (c+a, -r+b)
                k = count_cycles(n, n, lambda r, c: ((c + a) % n, (n - r + b) % n))
                total += pw2[k]

                # 270-degree: (r,c) -> (-c+a, r+b)
                k = count_cycles(n, n, lambda r, c: ((n - c + a) % n, (r + b) % n))
                total += pw2[k]

    # Burnside average
    ans = total // group_size
    print(ans)


if __name__ == "__main__":
    solve()
