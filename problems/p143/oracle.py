import sys
sys.setrecursionlimit(1000000)

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    # Read profits
    a = [int(next(it)) for _ in range(n)]
    # Build adjacency list
    adj = [[] for _ in range(n)]
    for _ in range(n - 1):
        u = int(next(it)) - 1
        v = int(next(it)) - 1
        adj[u].append(v)
        adj[v].append(u)
    
    # Global variable to store the best answer
    ans = -10**18

    # DFS returns best sum of a connected subtree rooted at u
    def dfs(u, parent):
        nonlocal ans
        subtotal = 0
        # Visit children
        for v in adj[u]:
            if v == parent:
                continue
            contrib = dfs(v, u)
            # Only add positive contributions
            if contrib > 0:
                subtotal += contrib
        current = subtotal + a[u]
        # Update global maximum
        ans = max(ans, current)
        # Return zero if negative, to prune bad branches
        return current if current > 0 else 0

    # Run DFS from node 0
    dfs(0, -1)
    # Print the result
    print(ans)

if __name__ == "__main__":
    main()
