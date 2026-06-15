import sys
from collections import deque

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    M = int(next(it))
    N = int(next(it))
    A = [0] * N
    B = [0] * N
    for i in range(N):
        A[i] = int(next(it))
        B[i] = int(next(it))

    # If nobody can produce balloons, impossible unless M==0 (but M>=1).
    if all(a == 0 for a in A):
        print(-1)
        return

    # ---- Precompute combos (subsets of size <= 4) and mapping mask -> combo index ----
    combos = []                 # combos[ci] = list of indices in the subset
    combo_index = [-1] * (1 << N)

    for mask in range(1 << N):
        if mask.bit_count() <= 4:
            combo_index[mask] = len(combos)
            positions = [i for i in range(N) if (mask >> i) & 1]
            combos.append(positions)

    num_combos = len(combos)

    # Encode/decode rest times for indices in positions using 2 bits each.
    # Stored value is (rest-1) in 0..3.
    def encode_state(rest, positions):
        v = 0
        for j, idx in enumerate(positions):
            v |= (rest[idx] - 1) << (2 * j)
        return v

    def decode_state(v, positions):
        rest = [0] * N
        for j, idx in enumerate(positions):
            rest[idx] = ((v >> (2 * j)) & 3) + 1
        return rest

    # available[ci][v] = list of usable volunteers when in (ci,v)
    available = [[[] for _ in range(256)] for __ in range(num_combos)]
    # adj[ci][v][used+1] -> (new_ci,new_v), used=-1 means idle
    adj = [[[None] * (N + 1) for _ in range(256)] for __ in range(num_combos)]

    # Build transitions for each (ci,v) that is valid for that combo size
    for ci in range(num_combos):
        positions = combos[ci]
        sz = len(positions)
        max_v = 1 << (2 * sz)   # equals 4^sz

        for v in range(max_v):
            rest = decode_state(v, positions)

            # Determine available volunteers (not resting and A[i]>0)
            avail = []
            for i in range(N):
                if rest[i] == 0 and A[i] > 0:
                    avail.append(i)
            available[ci][v] = avail

            # Precompute transitions for idle and for using each volunteer
            for used in range(-1, N):
                new_rest = [0] * N
                for i in range(N):
                    if i == used:
                        new_rest[i] = B[i]        # used volunteer starts cooldown
                    else:
                        new_rest[i] = rest[i] - 1 # time passes
                        if new_rest[i] < 0:
                            new_rest[i] = 0

                new_mask = 0
                for i in range(N):
                    if new_rest[i] > 0:
                        new_mask |= 1 << i

                new_ci = combo_index[new_mask]
                # new_mask must be size <= 4 for reachable states in this modeling
                # (the intended solution relies on that property).
                new_v = encode_state(new_rest, combos[new_ci])
                adj[ci][v][used + 1] = (new_ci, new_v)

        # For v >= max_v, (ci,v) is not a valid encoding; we leave as defaults.
        # BFS will never generate them.

    # ---- BFS over (balloons_left, ci, v) with unit cost per minute ----
    # dist[bal][ci][v] = minutes, use -1 for unvisited
    dist = [[[-1] * 256 for _ in range(num_combos)] for __ in range(M + 1)]

    q = deque()
    dist[M][0][0] = 0
    q.append((M, 0, 0))

    while q:
        bal, ci, v = q.popleft()
        d = dist[bal][ci][v]

        if bal == 0:
            print(d)
            return

        avail = available[ci][v]
        if not avail:
            # Must idle
            new_ci, new_v = adj[ci][v][0]
            if dist[bal][new_ci][new_v] == -1:
                dist[bal][new_ci][new_v] = d + 1
                q.append((bal, new_ci, new_v))
        else:
            # Try using each available volunteer
            for i in avail:
                new_ci, new_v = adj[ci][v][i + 1]
                new_bal = bal - A[i]
                if new_bal < 0:
                    new_bal = 0
                if dist[new_bal][new_ci][new_v] == -1:
                    dist[new_bal][new_ci][new_v] = d + 1
                    q.append((new_bal, new_ci, new_v))

    print(-1)

if __name__ == "__main__":
    solve()
