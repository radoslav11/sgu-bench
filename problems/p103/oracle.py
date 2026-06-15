import sys
import heapq

def read_input():
    data = sys.stdin.read().split()
    it = iter(data)
    s = int(next(it)) - 1
    d = int(next(it)) - 1
    n = int(next(it))
    m = int(next(it))
    # junctions[i] = (is_blue_initial, rem_initial, dur_blue, dur_purple)
    junctions = []
    for _ in range(n):
        C = next(it)
        r = int(next(it))
        tB = int(next(it))
        tP = int(next(it))
        junctions.append((C == 'B', r, tB, tP))
    adj = [[] for _ in range(n)]
    for _ in range(m):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        w = int(next(it))
        adj[u].append((v,w))
        adj[v].append((u,w))
    return s, d, n, adj, junctions

def get_color(u, t, junctions):
    is_blue0, rem0, tB, tP = junctions[u]
    # if before initial switch
    if t < rem0:
        return 1 if is_blue0 else 0
    t -= rem0
    cycle = tB + tP
    tt = t % cycle
    # if initial was blue, next block is purple
    if is_blue0:
        if tt < tP:      # in initial purple
            return 0
        tt -= tP
        return 1 if tt < tB else 0
    else:
        if tt < tB:      # in initial blue
            return 1
        tt -= tB
        return 0 if tt < tP else 1

def time_to_next_change(u, t, junctions):
    is_blue0, rem0, tB, tP = junctions[u]
    if t < rem0:
        return rem0 - t
    t -= rem0
    cycle = tB + tP
    tt = t % cycle
    if is_blue0:
        # purple block
        if tt < tP:
            return tP - tt
        tt -= tP
        # blue block
        if tt < tB:
            return tB - tt
    else:
        # blue block
        if tt < tB:
            return tB - tt
        tt -= tB
        # purple block
        if tt < tP:
            return tP - tt
    # fallback
    return cycle - tt

def first_sync_time(u, v, t, junctions):
    # at most 3 trials
    for _ in range(3):
        if get_color(u, t, junctions) == get_color(v, t, junctions):
            return t
        # wait for next change at u or v
        du = time_to_next_change(u, t, junctions)
        dv = time_to_next_change(v, t, junctions)
        t += min(du, dv)
    return -1

def dijkstra(s, d, n, adj, junctions):
    INF = 10**18
    dist = [INF]*n
    parent = [-1]*n
    dist[s] = 0
    pq = [(0, s)]
    while pq:
        t, u = heapq.heappop(pq)
        if t > dist[u]:
            continue
        for v, w in adj[u]:
            depart = first_sync_time(u, v, t, junctions)
            if depart < 0:
                continue
            arrive = depart + w
            if arrive < dist[v]:
                dist[v] = arrive
                parent[v] = u
                heapq.heappush(pq, (arrive, v))
    return dist, parent

def main():
    s, d, n, adj, junctions = read_input()
    dist, parent = dijkstra(s, d, n, adj, junctions)
    if dist[d] == 10**18:
        print(0)
        return
    # reconstruct path
    path = []
    cur = d
    while cur != -1:
        path.append(cur+1)  # 1-based
        cur = parent[cur]
    path.reverse()
    print(dist[d])
    print(*path)

if __name__ == "__main__":
    main()
