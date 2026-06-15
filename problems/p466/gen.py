"""Tests for p466 (Parking at Secret Object).

Input: N Q, a string of '.'/'X' of length N, then Q queries "PARK S"
(1 <= S <= N) or "LEAVE i" referencing an earlier still-parked
successful PARK. The statement's numeric limits are lost; tests use
N, Q <= 100000.

The generator replays the allocation rules (best-fit cluster, min size
then min head, circular merging on leave) so every LEAVE it emits is
consistent. Seed 1 is the 1-slot case, seed 2 max-size churn from an
empty circle, seed 3 max-size with 50000 single-slot clusters.
"""

import bisect
import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


class Sim:
    """Mirror of the parking logic: clusters kept sorted by (size, head)
    for allocation and by head for circular neighbour merging."""

    def __init__(self, n, slots):
        self.n = n
        self.by_size = []
        self.by_pos = []
        self.info = {}
        self.next_id = 0
        self.alloc = {}
        occ = next((i + 1 for i, ch in enumerate(slots) if ch == "X"), None)
        if occ is None:
            self.add(1, n)
        else:
            i = occ
            while True:
                if slots[i - 1] == ".":
                    h, length = i, 0
                    while slots[i - 1] == ".":
                        length += 1
                        i = self.norm(i + 1)
                    self.add(h, length)
                else:
                    i = self.norm(i + 1)
                if i == occ:
                    break

    def norm(self, p):
        if p <= 0:
            return p + self.n
        if p > self.n:
            return p - self.n
        return p

    def add(self, head, size):
        cid = self.next_id
        self.next_id += 1
        bisect.insort(self.by_size, (size, head, cid))
        bisect.insort(self.by_pos, (head, cid))
        self.info[cid] = (head, size)

    def remove(self, cid):
        head, size = self.info.pop(cid)
        self.by_size.pop(bisect.bisect_left(self.by_size, (size, head, cid)))
        self.by_pos.pop(bisect.bisect_left(self.by_pos, (head, cid)))

    def park(self, q, s):
        i = bisect.bisect_left(self.by_size, (s,))
        if i == len(self.by_size):
            return False
        size, head, cid = self.by_size[i]
        self.alloc[q] = (head, s)
        self.remove(cid)
        if size > s:
            self.add(self.norm(head + s), size - s)
        return True

    def leave(self, q):
        head, size = self.alloc.pop(q)
        if not self.by_pos:
            self.add(head, size)
            return
        end = self.norm(head + size)
        j = bisect.bisect_left(self.by_pos, (end,))
        if j == len(self.by_pos):
            j = 0
        k = j - 1 if j > 0 else len(self.by_pos) - 1
        left_id, right_id = self.by_pos[k][1], self.by_pos[j][1]
        lhead, lsize = self.info[left_id]
        rhead, rsize = self.info[right_id]
        ml = self.norm(lhead + lsize) == head
        mr = self.norm(head + size) == rhead
        if ml:
            head, size = lhead, size + lsize
            self.remove(left_id)
        if mr and right_id in self.info:
            size += rsize
            self.remove(right_id)
        if size == self.n:
            head = 1
        self.add(head, size)


def gen_queries(n, q, slots, size_pick, leave_prob):
    sim = Sim(n, slots)
    active = []
    lines = []
    for idx in range(1, q + 1):
        if active and rng.random() < leave_prob:
            i = rng.randrange(len(active))
            active[i], active[-1] = active[-1], active[i]
            ref = active.pop()
            sim.leave(ref)
            lines.append(f"LEAVE {ref}")
        else:
            s = size_pick()
            if sim.park(idx, s):
                active.append(idx)
            lines.append(f"PARK {s}")
    return lines


def emit(n, slots, lines):
    print(n, len(lines))
    print(slots)
    print("\n".join(lines))


if seed == 1:
    emit(1, ".", ["PARK 1"])
elif seed == 2:
    n = q = 100000
    slots = "." * n
    emit(n, slots, gen_queries(n, q, slots, lambda: rng.randint(1, 6), 0.4))
elif seed == 3:
    n = q = 100000
    slots = "X." * (n // 2)
    emit(
        n,
        slots,
        gen_queries(
            n,
            q,
            slots,
            lambda: rng.choice([1, 1, 1, 2, n // 2, n]),
            0.45,
        ),
    )
elif seed == 4:
    n, q = 100000, 100000
    slots = ["."] * n
    for _ in range(n // 4):
        slots[rng.randrange(n)] = "X"
    slots[0] = "."
    slots[-1] = "."
    slots = "".join(slots)
    emit(
        n,
        slots,
        gen_queries(n, q, slots, lambda: rng.randint(1, 40), 0.35),
    )
elif seed == 5:
    n, q = 10, 11
    slots = "....X..X.."
    emit(
        n,
        slots,
        [
            "PARK 1",
            "PARK 3",
            "PARK 4",
            "LEAVE 2",
            "PARK 5",
            "LEAVE 5",
            "PARK 1",
            "PARK 1",
            "PARK 2",
            "PARK 4",
            "PARK 3",
        ],
    )
else:
    n = rng.randint(2, 500)
    q = rng.randint(1, 500)
    density = rng.random()
    slots = "".join("X" if rng.random() < density else "." for _ in range(n))
    emit(
        n,
        slots,
        gen_queries(
            n,
            q,
            slots,
            lambda: rng.randint(1, max(1, int(n * rng.random()))),
            rng.uniform(0.2, 0.5),
        ),
    )
