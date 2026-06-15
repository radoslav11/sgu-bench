import sys


def read_tree():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    adj = [[] for _ in range(n)]
    for i in range(1, n):
        u = int(next(it)) - 1
        w = int(next(it))
        adj[i].append((u, w))
        adj[u].append((i, w))
    return n, adj


def get_distances(n, adj, src):
    dist = [-1] * n
    dist[src] = 0
    stack = [src]
    while stack:
        u = stack.pop()
        for v, w in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + w
                stack.append(v)
    return dist


def main():
    n, adj = read_tree()
    dist0 = get_distances(n, adj, 0)
    u = max(range(n), key=lambda i: dist0[i])
    dist_u = get_distances(n, adj, u)
    v = max(range(n), key=lambda i: dist_u[i])
    dist_v = get_distances(n, adj, v)
    out = "\n".join(str(max(dist_u[i], dist_v[i])) for i in range(n))
    print(out)


if __name__ == "__main__":
    main()
