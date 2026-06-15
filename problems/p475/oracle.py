import sys
import heapq

INF = 10**30

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    # Participant parameters
    w = [0] * n  # weight
    t = [0] * n  # walking time per segment
    s = [0] * n  # swap time to get on/off raft
    for j in range(n):
        w[j] = int(next(it))
        t[j] = int(next(it))
        s[j] = int(next(it))

    # Riffle parameters
    c = [0] * m  # critical weight
    D = [0] * m  # time if capsizes
    d = [0] * m  # time if not
    for i in range(m):
        c[i] = int(next(it))
        D[i] = int(next(it))
        d[i] = int(next(it))

    N = 1 << n  # number of masks

    # Precompute:
    # sum_w[mask]  = total weight of people on raft
    # max_out[mask]= max walking time among walkers (people NOT in mask)
    sum_w = [0] * N
    max_out = [0] * N
    for mask in range(N):
        sw = 0
        mo = 0
        for j in range(n):
            if mask & (1 << j):
                sw += w[j]
            else:
                if t[j] > mo:
                    mo = t[j]
        sum_w[mask] = sw
        max_out[mask] = mo

    def run_dijkstra(dist):
        """
        Dijkstra on the mask graph:
        node = mask
        edge: mask -> mask^(1<<j) with cost s[j]
        Starts with initial distances in 'dist' and relaxes all masks.
        """
        pq = []
        for mask in range(N):
            if dist[mask] < INF:
                heapq.heappush(pq, (dist[mask], mask))

        while pq:
            cur, umask = heapq.heappop(pq)
            if cur != dist[umask]:
                continue

            # Try toggling each bit j
            for j in range(n):
                vmask = umask ^ (1 << j)
                nd = cur + s[j]
                if nd < dist[vmask]:
                    dist[vmask] = nd
                    heapq.heappush(pq, (nd, vmask))

    # DP over points: prev_dp[mask] = best time at current point with raft set mask
    prev_dp = [INF] * N
    prev_dp[0] = 0  # start with everyone on bank

    for i in range(m):
        # Allow any swapping at point p_{i} before traversing riffle i+1
        dist = prev_dp[:]        # copy
        run_dijkstra(dist)

        # Traverse the riffle with a non-empty raft
        next_dp = [INF] * N
        for mask in range(1, N):
            if dist[mask] >= INF:
                continue

            capsize = sum_w[mask] > c[i]
            raft_t = D[i] if capsize else d[i]

            # segment finishes when raft and all walkers arrive
            seg_t = raft_t if raft_t > max_out[mask] else max_out[mask]

            next_dp[mask] = dist[mask] + seg_t

        prev_dp = next_dp

    # At finish point pm, allow swaps to get everyone off (reach mask=0)
    dist = prev_dp[:]
    run_dijkstra(dist)

    print(dist[0])

if __name__ == "__main__":
    solve()
