import sys
from typing import List, Tuple

def build_events(times: List[int], t: int) -> List[Tuple[int, int]]:
    """
    Convert sorted pass times into merged open intervals for parameter t,
    returned as sorted events (pos, delta), delta in {+1, -1}.
    """
    events = []
    i = 0
    n = len(times)

    while i < n:
        start = times[i] - t
        end = times[i] + t
        i += 1

        # Merge while next interval overlaps current:
        # [times[i]-t, times[i]+t] overlaps [start,end] iff times[i]-t <= end
        while i < n and times[i] - t <= end:
            end = times[i] + t
            i += 1

        # Record interval as events
        events.append((start, +1))
        events.append((end, -1))

    # events are already in nondecreasing order of position because times are sorted
    return events

def check(p: List[int], q: List[int], d: int, t1: int, t2: int) -> Tuple[bool, int]:
    """
    Returns (is_valid, total_openings) for given (t1, t2).
    is_valid: no overlap segment (both doors open) has length > d.
    total_openings: number of merged segments in door1 + door2.
    """
    pe = build_events(p, t1)
    qe = build_events(q, t2)

    openings = len(pe)//2 + len(qe)//2

    i = j = 0
    balance = 0          # number of open doors right now
    last_pos = None      # start position of current overlap (balance==2), else None

    # Merge two sorted event lists with tie-breaking:
    # smaller position first; if equal, process delta=-1 before delta=+1.
    while i < len(pe) or j < len(qe):
        if j >= len(qe):
            pos, delta = pe[i]
            i += 1
        elif i >= len(pe):
            pos, delta = qe[j]
            j += 1
        else:
            # compare (pos, delta) lexicographically; delta -1 < +1 enforces "close before open"
            if pe[i] < qe[j]:
                pos, delta = pe[i]
                i += 1
            else:
                pos, delta = qe[j]
                j += 1

        balance += delta

        if balance == 2:
            # entering overlap
            last_pos = pos
        else:
            # leaving overlap: after leaving, balance becomes 1 (because only one door remains open)
            if balance == 1 and last_pos is not None and (pos - last_pos) > d:
                return False, openings
            last_pos = None

    return True, openings

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it)); m = int(next(it)); d = int(next(it))
    p = [int(next(it)) for _ in range(n)]
    q = [int(next(it)) for _ in range(m)]

    # Candidate t values where merging structure can change: ceil(gap/2)
    cand1 = []
    for i in range(1, n):
        gap = p[i] - p[i-1]
        cand1.append((gap + 1)//2)

    cand2 = []
    for i in range(1, m):
        gap = q[i] - q[i-1]
        cand2.append((gap + 1)//2)

    # Guard values
    cand1.extend([1, d, (d + 1)//2])
    cand2.extend([1, d, (d + 1)//2])

    cand1 = sorted(set(cand1))
    cand2 = sorted(set(cand2))

    best_openings = n + m + 1
    best_t1 = best_t2 = None

    # Two-pointers: start with largest feasible t2 for each t1
    j = len(cand2) - 1
    for t1 in cand1:
        # decrease t2 until pair becomes valid
        while j >= 0:
            ok, openings = check(p, q, d, t1, cand2[j])
            if ok:
                break
            j -= 1

        if j >= 0:
            if openings < best_openings:
                best_openings = openings
                best_t1 = t1
                best_t2 = cand2[j]

    if best_t1 is None:
        sys.stdout.write("No solution\n")
    else:
        sys.stdout.write(f"{best_t1} {best_t2}\n")

if __name__ == "__main__":
    solve()
