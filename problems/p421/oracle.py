import sys
import heapq
from dataclasses import dataclass

# We will store products as Python int (unbounded precision).


@dataclass(frozen=True)
class Key:
    """Immutable key used for visited-set membership."""
    neg: tuple
    pos: tuple


class State:
    """
    Mutable state stored in heap:
    - neg_indices, pos_indices are lists of chosen indices
    - boundaries define valid ranges for index moves
    - product is computed once and stored
    """
    __slots__ = ("neg_indices", "pos_indices",
                 "neg_lo", "neg_hi", "pos_lo", "pos_hi",
                 "prod")

    def __init__(self, neg_indices, pos_indices, neg_lo, neg_hi, pos_lo, pos_hi, prod):
        self.neg_indices = neg_indices
        self.pos_indices = pos_indices
        self.neg_lo = neg_lo
        self.neg_hi = neg_hi
        self.pos_lo = pos_lo
        self.pos_hi = pos_hi
        self.prod = prod  # cached product (Python int)

    def key(self):
        return Key(tuple(self.neg_indices), tuple(self.pos_indices))


def create_initial_state(a, n, m, split, num_negs):
    """
    Mirrors the C++ create_initial_state logic.
    """
    minimize_abs = (num_negs % 2 == 0)
    num_pos = m - num_negs

    if minimize_abs:
        # positives: largest values at end
        pos_indices = [n - 1 - i for i in range(num_pos)]
        # negatives: from start
        neg_indices = [i for i in range(num_negs)]
    else:
        # positives: smallest non-negatives
        pos_indices = [split + i for i in range(num_pos)]
        # negatives: near split-1 backward
        neg_indices = [split - 1 - i for i in range(num_negs)]

    neg_lo, neg_hi = 0, split
    pos_lo, pos_hi = split, n

    # compute product
    prod = 1
    for idx in neg_indices:
        prod *= a[idx]
    for idx in pos_indices:
        prod *= a[idx]

    return State(neg_indices, pos_indices, neg_lo, neg_hi, pos_lo, pos_hi, prod)


def solve():
    it = iter(sys.stdin.read().strip().split())
    n = int(next(it)); m = int(next(it)); k = int(next(it))
    a = [int(next(it)) for _ in range(n)]

    a.sort()

    # split point: first non-negative
    split = 0
    while split < n and a[split] < 0:
        split += 1

    total_neg = split
    total_pos = n - split

    # Python heapq is a min-heap, so to simulate max-heap we push (-prod, counter, state).
    heap = []
    visited = set()
    counter = 0  # ensures tie-breaking without comparing State objects

    # Push initial states for all feasible numbers of negatives
    for num_negs in range(m + 1):
        num_pos = m - num_negs
        if num_negs > total_neg or num_pos > total_pos:
            continue
        st = create_initial_state(a, n, m, split, num_negs)
        key = st.key()
        if key not in visited:
            visited.add(key)
            heapq.heappush(heap, (-st.prod, counter, st))
            counter += 1

    # Pop k times
    for step in range(k):
        if not heap:
            break
        negprod, _, cur = heapq.heappop(heap)
        if step == k - 1:
            print(cur.prod)
            return

        # Generate neighbors by moving one pos index by ±1, maintaining strict order
        if cur.pos_indices:
            moving_up = cur.pos_indices[0] < cur.pos_indices[-1]
            for i in range(len(cur.pos_indices)):
                cur_val = cur.pos_indices[i]
                new_val = cur_val + 1 if moving_up else cur_val - 1

                if i + 1 < len(cur.pos_indices):
                    limit = cur.pos_indices[i + 1]
                else:
                    limit = cur.pos_hi if moving_up else cur.pos_lo - 1

                valid = (new_val < limit) if moving_up else (new_val > limit)
                if not valid:
                    continue

                # Build neighbor state (copy lists)
                pos2 = cur.pos_indices[:]
                pos2[i] = new_val

                # recompute product cheaply by dividing/multiplying (exact division holds)
                if a[cur_val] != 0:
                    prod2 = cur.prod // a[cur_val] * a[new_val]
                else:
                    prod2 = 1
                    for idx in cur.neg_indices:
                        prod2 *= a[idx]
                    for idx in pos2:
                        prod2 *= a[idx]

                nb = State(cur.neg_indices[:], pos2,
                           cur.neg_lo, cur.neg_hi, cur.pos_lo, cur.pos_hi,
                           prod2)
                key = nb.key()
                if key not in visited:
                    visited.add(key)
                    heapq.heappush(heap, (-nb.prod, counter, nb))
                    counter += 1

        # Generate neighbors by moving one neg index similarly
        if cur.neg_indices:
            moving_up = cur.neg_indices[0] < cur.neg_indices[-1]
            for i in range(len(cur.neg_indices)):
                cur_val = cur.neg_indices[i]
                new_val = cur_val + 1 if moving_up else cur_val - 1

                if i + 1 < len(cur.neg_indices):
                    limit = cur.neg_indices[i + 1]
                else:
                    limit = cur.neg_hi if moving_up else cur.neg_lo - 1

                valid = (new_val < limit) if moving_up else (new_val > limit)
                if not valid:
                    continue

                neg2 = cur.neg_indices[:]
                neg2[i] = new_val

                if a[cur_val] != 0:
                    prod2 = cur.prod // a[cur_val] * a[new_val]
                else:
                    prod2 = 1
                    for idx in neg2:
                        prod2 *= a[idx]
                    for idx in cur.pos_indices:
                        prod2 *= a[idx]

                nb = State(neg2, cur.pos_indices[:],
                           cur.neg_lo, cur.neg_hi, cur.pos_lo, cur.pos_hi,
                           prod2)
                key = nb.key()
                if key not in visited:
                    visited.add(key)
                    heapq.heappush(heap, (-nb.prod, counter, nb))
                    counter += 1


if __name__ == "__main__":
    solve()
