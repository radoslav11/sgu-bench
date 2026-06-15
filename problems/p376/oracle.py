import sys

def solve() -> None:
    data = list(map(int, sys.stdin.buffer.read().split()))
    it = iter(data)

    n = next(it)
    k = next(it)

    # Store participant fields in parallel arrays for speed.
    # id is 1..n
    swim = [0] * n        # swim time = 2e7 - Wi
    ride_base = [0] * n   # ride base = 2e7 - Ri (horse value is subtracted)
    strength = [0] * n    # Pi
    total = [0] * n       # total time so far

    for i in range(n):
        w = next(it)
        r = next(it)
        p = next(it)
        swim[i] = 20_000_000 - w
        ride_base[i] = 20_000_000 - r
        strength[i] = p
        total[i] = swim[i]

    # Read arrays A (size K) and B (size N)
    A = [next(it) for _ in range(k)]
    B = [next(it) for _ in range(n)]

    # Precompute sorted horse values per stage (descending).
    # stage_horses[s][pos] = pos-th best horse value at stage s.
    stage_horses = []
    for s in range(k):
        a = A[s]
        horses = []
        # Compute f(s, j) for all j
        for bj in B:
            horses.append(3*a*a + 5*a*bj + 2*bj*bj)
        horses.sort(reverse=True)
        stage_horses.append(horses)

    # arrival time at current stage start (initially after swim)
    arrival = swim[:]

    # Simulate each riding stage
    for s in range(k):
        # Order participants by arrival asc, strength desc
        order = list(range(n))
        order.sort(key=lambda i: (arrival[i], -strength[i]))

        new_arrival = [0] * n
        horses = stage_horses[s]

        # Assign pos-th horse to pos-th participant in that order
        for pos, pi in enumerate(order):
            horse_val = horses[pos]
            ride_time = ride_base[pi] - horse_val
            finish = arrival[pi] + ride_time
            new_arrival[pi] = finish
            total[pi] = finish

        arrival = new_arrival

    # Final ranking: total time asc, strength desc
    final_order = list(range(n))
    final_order.sort(key=lambda i: (total[i], -strength[i]))

    # Output participant IDs (1-based)
    ans = [str(i + 1) for i in final_order]
    sys.stdout.write(" ".join(ans))

if __name__ == "__main__":
    solve()
