"""Tests for p263 (Towers): up to 1e6 commands over cells 1..1e6.

The generator simulates the occupied-cell intervals itself (Fenwick
tree over interval starts) so that every tower number and column it
emits is valid, and per-cell cube totals stay below 2^31-1.

Seed 1 is minimal (one 'towers' query). Seed 2 is maximal: clustered
random puts causing many merges, with all query kinds mixed in. Seed 3
is maximal with a different shape: half a million isolated towers
created at even cells, then merged pairwise by filling odd cells,
query-heavy. Seed 4 builds one giant contiguous tower left to right.
Later seeds are smaller random mixes.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

MAXC = 10**6
LIMIT = 2**31 - 1


class Sim:
    def __init__(self):
        self.occ = bytearray(MAXC + 2)
        self.other = [0] * (MAXC + 2)
        self.cubes = [0] * (MAXC + 2)
        self.bit = [0] * (MAXC + 1)
        self.towers = 0
        self.log = 20  # 2^20 > 1e6

    def _upd(self, i, v):
        while i <= MAXC:
            self.bit[i] += v
            i += i & (-i)

    def _kth(self, k):
        # position of k-th interval start (1-based)
        pos = 0
        rem = k
        for j in range(self.log, -1, -1):
            npos = pos + (1 << j)
            if npos <= MAXC and self.bit[npos] < rem:
                pos = npos
                rem -= self.bit[npos]
        return pos + 1

    def put(self, x, c):
        c = min(c, LIMIT - self.cubes[x])
        c = max(c, 1)
        self.cubes[x] += c
        if not self.occ[x]:
            self.occ[x] = 1
            left = x > 1 and self.occ[x - 1]
            right = x < MAXC and self.occ[x + 1]
            if left and right:
                a, b = self.other[x - 1], self.other[x + 1]
                self._upd(x + 1, -1)
                self.towers -= 1
                self.other[a], self.other[b] = b, a
            elif left:
                a = self.other[x - 1]
                self.other[a], self.other[x] = x, a
            elif right:
                b = self.other[x + 1]
                self._upd(x + 1, -1)
                self._upd(x, 1)
                self.other[x], self.other[b] = b, x
            else:
                self._upd(x, 1)
                self.towers += 1
                self.other[x] = x
        return c

    def tower(self, t):
        s = self._kth(t)
        return s, self.other[s] - s + 1


def gen_commands(n, cell_picker, weights):
    sim = Sim()
    out = []
    kinds = ["put", "tput", "towers", "cubes", "length", "tcubes"]
    while len(out) < n:
        kind = rng.choices(kinds, weights=weights)[0]
        if kind != "put" and kind != "towers" and sim.towers == 0:
            kind = "put"
        if kind == "put":
            x = cell_picker()
            c = sim.put(x, rng.randint(1, 1000))
            out.append(f"put {x} {c}")
        elif kind == "tput":
            t = rng.randint(1, sim.towers)
            s, ln = sim.tower(t)
            x = rng.randint(1, ln)
            c = sim.put(s + x - 1, rng.randint(1, 1000))
            out.append(f"tput {t} {x} {c}")
        elif kind == "towers":
            out.append("towers")
        elif kind == "cubes":
            out.append(f"cubes {rng.randint(1, sim.towers)}")
        elif kind == "length":
            out.append(f"length {rng.randint(1, sim.towers)}")
        else:
            t = rng.randint(1, sim.towers)
            s, ln = sim.tower(t)
            out.append(f"tcubes {t} {rng.randint(1, ln)}")
    return out


def emit(cmds):
    sys.stdout.write(str(len(cmds)) + "\n" + "\n".join(cmds) + "\n")


if seed == 1:
    emit(["towers"])
elif seed == 2:
    centers = [rng.randint(1, MAXC) for _ in range(200)]

    def picker():
        if rng.random() < 0.8:
            return min(MAXC, max(1, rng.choice(centers) + rng.randint(-300, 300)))
        return rng.randint(1, MAXC)

    emit(gen_commands(10**6, picker, [30, 25, 5, 15, 10, 15]))
elif seed == 3:
    sim = Sim()
    out = []
    n = 10**6
    evens = list(range(2, MAXC, 2))[:400000]
    for x in evens:
        c = sim.put(x, rng.randint(1, 1000))
        out.append(f"put {x} {c}")
    out.append("towers")
    odds = evens[:]
    rng.shuffle(odds)
    for x in odds:
        if len(out) >= n:
            break
        y = x + 1
        c = sim.put(y, rng.randint(1, 1000))
        out.append(f"put {y} {c}")
        if sim.towers and rng.random() < 0.4:
            t = rng.randint(1, sim.towers)
            kind = rng.randrange(3)
            if kind == 0:
                out.append(f"cubes {t}")
            elif kind == 1:
                out.append(f"length {t}")
            else:
                s, ln = sim.tower(t)
                out.append(f"tcubes {t} {rng.randint(1, ln)}")
    while len(out) < n:
        out.append("towers")
    emit(out[:n])
elif seed == 4:
    out = []
    n = 10**6
    pos = 1
    total = 0
    while len(out) < n - 1:
        c = rng.randint(1, 1000)
        out.append(f"put {pos} {c}")
        pos += 1
        if pos > MAXC:
            break
        if rng.random() < 0.5:
            out.append("cubes 1")
    out.append("length 1")
    emit(out)
else:
    n = rng.randint(2, 5000)

    def picker():
        return rng.randint(1, rng.choice([50, 1000, MAXC]))

    emit(gen_commands(n, picker, [30, 20, 10, 15, 10, 15]))
