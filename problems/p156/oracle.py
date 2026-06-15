import sys
import threading

sys.setrecursionlimit(10**7)


def main():
    data = sys.stdin.buffer.read().split()
    n, m = int(data[0]), int(data[1])
    adj = [[] for _ in range(n)]
    for i in range(m):
        u = int(data[2 + 2 * i]) - 1
        v = int(data[3 + 2 * i]) - 1
        adj[u].append(v)
        adj[v].append(u)

    visited_hub = [False] * n
    next_node = [-1] * n

    def match_nodes(u):
        visited_hub[u] = True
        cnt = 1
        for v in adj[u]:
            if len(adj[v]) == 2:
                next_node[u] = v
            elif len(adj[v]) > 2 and not visited_hub[v]:
                cnt += match_nodes(v)
        return cnt

    for i in range(n):
        if len(adj[i]) > 2 and not visited_hub[i]:
            c = match_nodes(i)
            if c % 2 == 1:
                print(-1)
                return

    visited = [False] * n
    hcycle = []

    def dfs(u, in_path):
        visited[u] = True
        if len(adj[u]) > 2:
            w = next_node[u]
            if w != -1 and not visited[w]:
                dfs(w, False)
        for v in adj[u]:
            if not visited[v]:
                if len(adj[u]) == 2:
                    dfs(v, False)
                elif not in_path and len(adj[v]) > 2:
                    dfs(v, True)
        hcycle.append(u)

    dfs(0, False)

    if len(hcycle) != n:
        print(-1)
    else:
        hcycle.reverse()
        print(" ".join(str(u + 1) for u in hcycle))


if __name__ == "__main__":
    threading.stack_size(256 * 1024 * 1024)
    t = threading.Thread(target=main)
    t.start()
    t.join()
