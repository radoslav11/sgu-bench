import sys
import heapq


def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return

    ptr = 0
    n = int(data[ptr])
    ptr += 1
    total_time = int(data[ptr])
    ptr += 1

    t = [0] * (n + 1)
    for i in range(1, n + 1):
        t[i] = int(data[ptr])
        ptr += 1

    m = int(data[ptr])
    ptr += 1

    adj = [[] for _ in range(n + 1)]
    indeg = [0] * (n + 1)
    for _ in range(m):
        a = int(data[ptr])
        ptr += 1
        b = int(data[ptr])
        ptr += 1
        adj[a].append(b)
        indeg[b] += 1

    heap = []
    for i in range(1, n + 1):
        if indeg[i] == 0:
            heapq.heappush(heap, (t[i], i))

    order = []
    elapsed = 0
    penalty = 0
    while heap:
        ti, u = heapq.heappop(heap)
        if elapsed + ti > total_time:
            break
        elapsed += ti
        penalty += elapsed
        order.append(u)
        for v in adj[u]:
            indeg[v] -= 1
            if indeg[v] == 0:
                heapq.heappush(heap, (t[v], v))

    out = []
    out.append(f"{len(order)} {penalty}")
    out.append(" ".join(map(str, order)))
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    solve()
