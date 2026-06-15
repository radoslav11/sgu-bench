import sys
from math import inf

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    n = int(next(it))
    m = int(next(it))
    r = int(next(it))

    # Probabilities p_i are integers summing to 10000 (hundredths of percent)
    prob = [int(next(it)) for _ in range(n)]

    # dist[i][j] will hold all-pairs shortest path distances
    dist = [[inf] * n for _ in range(n)]
    for i in range(n):
        dist[i][i] = 0.0

    edges = []
    for _ in range(m):
        a = int(next(it)) - 1
        b = int(next(it)) - 1
        L = int(next(it))
        edges.append((a, b, L))
        # Keep minimal length if repeated (usually unnecessary)
        if L < dist[a][b]:
            dist[a][b] = dist[b][a] = float(L)

    # Floyd–Warshall: O(n^3), fine for n<=100
    for k in range(n):
        dk = dist[k]
        for i in range(n):
            dik = dist[i][k]
            if dik == inf:
                continue
            di = dist[i]
            # Relax j via k
            for j in range(n):
                nd = dik + dk[j]
                if nd < di[j]:
                    di[j] = nd

    # If graph disconnected, impossible to reach all cities from any station
    for i in range(n):
        for j in range(n):
            if dist[i][j] == inf:
                print(-1)
                return

    best = inf  # best expected cost in (probability units) * km

    # If R=0, station can be built in a city
    if r == 0:
        for c in range(n):
            cost = 0.0
            dc = dist[c]
            for x in range(n):
                cost += prob[x] * dc[x]
            if cost < best:
                best = cost

    # Station can also be built on roads, at distance d from one endpoint.
    # For each edge, only need to test d=R and d=L-R (concavity argument).
    for a, b, L in edges:
        if L < 2 * r:
            continue  # no feasible point on this road
        for d in (float(r), float(L - r)):
            cost = 0.0
            da = dist[a]
            db = dist[b]
            for x in range(n):
                # dist(station, x) = min(d + dist[a][x], (L-d) + dist[b][x])
                via_a = d + da[x]
                via_b = (L - d) + db[x]
                cost += prob[x] * (via_a if via_a < via_b else via_b)
            if cost < best:
                best = cost

    if best == inf:
        print(-1)
    else:
        # Divide by 10000 to convert from integer probability units to expectation.
        # Print with 5 decimals as in the reference solution/output.
        print(f"{best / 10000.0:.5f}")

if __name__ == "__main__":
    solve()
