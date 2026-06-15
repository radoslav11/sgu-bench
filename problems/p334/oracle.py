import sys
from itertools import permutations

# ---------- DSU (Union-Find) ----------
class DSU:
    def __init__(self, n: int):
        # We will use indices 0..n-1
        self.par = list(range(n))
        self.sz = [1] * n

    def root(self, x: int) -> int:
        # Path compression
        while self.par[x] != x:
            self.par[x] = self.par[self.par[x]]
            x = self.par[x]
        return x

    def unite(self, a: int, b: int) -> None:
        a, b = self.root(a), self.root(b)
        if a == b:
            return
        # Union by size: attach smaller to larger
        if self.sz[a] > self.sz[b]:
            a, b = b, a
        self.par[a] = b
        self.sz[b] += self.sz[a]


def solve() -> None:
    data = sys.stdin.read().strip().splitlines()
    h, w = map(int, data[0].split())
    grid = data[1:1+h]

    # Collect the 9 occupied cells
    cells = []
    for r in range(h):
        for c in range(w):
            if grid[r][c] == 'X':
                cells.append((r, c))

    # Build adjacency list among the 9 cells in the original figure
    adj = [[] for _ in range(9)]
    for i in range(9):
        r1, c1 = cells[i]
        for j in range(i + 1, 9):
            r2, c2 = cells[j]
            if abs(r1 - r2) + abs(c1 - c2) == 1:
                adj[i].append(j)
                adj[j].append(i)

    # Build DSU for a given mapping p[i] = target position (0..8) for input cell i
    def build_dsu(p):
        dsu = DSU(9)

        # Translation vectors needed for each cell i
        dr = [0] * 9
        dc = [0] * 9
        for i in range(9):
            tr, tc = divmod(p[i], 3)   # target row/col in 3x3
            r, c = cells[i]            # original position
            dr[i] = tr - r
            dc[i] = tc - c

        # Union adjacent original cells if they share the same translation
        for i in range(9):
            for j in adj[i]:
                if j > i and dr[i] == dr[j] and dc[i] == dc[j]:
                    dsu.unite(i, j)
        return dsu

    best_parts = 10
    best_perm = None

    # Try all 9! bijections from input cells to 3x3 target positions
    for p in permutations(range(9)):
        dsu = build_dsu(p)

        # Count components (roots)
        roots = set(dsu.root(i) for i in range(9))
        parts = len(roots)

        if parts < best_parts:
            best_parts = parts
            best_perm = p
            if best_parts == 1:
                break

    # Rebuild DSU for best permutation and assign letters to components
    dsu = build_dsu(best_perm)

    # Map DSU root -> label index 0..K-1
    root_to_label = {}
    next_label = 0
    for i in range(9):
        r = dsu.root(i)
        if r not in root_to_label:
            root_to_label[r] = next_label
            next_label += 1

    # Output K
    out = []
    out.append(str(best_parts))

    # Print original grid with X replaced by letters
    grid2 = [list(row) for row in grid]
    for i in range(9):
        r, c = cells[i]
        lab = root_to_label[dsu.root(i)]
        grid2[r][c] = chr(ord('A') + lab)
    out.extend("".join(row) for row in grid2)

    # Blank line
    out.append("")

    # Print 3x3 target arrangement
    small = [["."] * 3 for _ in range(3)]
    for i in range(9):
        tr, tc = divmod(best_perm[i], 3)
        lab = root_to_label[dsu.root(i)]
        small[tr][tc] = chr(ord('A') + lab)
    out.extend("".join(row) for row in small)

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
