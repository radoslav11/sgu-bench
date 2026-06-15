import sys
from collections import deque

sys.setrecursionlimit(10 ** 7)


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    m = int(data[idx]); idx += 1
    adj = [[] for _ in range(n)]
    for _ in range(m):
        u = int(data[idx]) - 1; idx += 1
        v = int(data[idx]) - 1; idx += 1
        adj[u].append(v)
        adj[v].append(u)

    dist = [-1] * n
    dist[0] = 0
    q = deque([0])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)

    dag = [[] for _ in range(n)]
    for u in range(n):
        for v in adj[u]:
            if dist[v] == dist[u] + 1:
                dag[u].append(v)

    dp = [-1] * n

    # iterative post-order: state 0 = entering, state 1 = children done
    def dfs(root):
        stack = [(root, 0)]
        while stack:
            node, state = stack.pop()
            if state == 0:
                if dp[node] != -1:
                    continue
                stack.append((node, 1))
                for v in dag[node]:
                    if dp[v] == -1:
                        stack.append((v, 0))
            else:
                dp[node] = 0
                for v in dag[node]:
                    if dp[v] == 0:
                        dp[node] = 1
                        break
        return dp[root]

    start = dfs(0)
    print("Vladimir" if start == 1 else "Nikolay")


if __name__ == "__main__":
    main()
