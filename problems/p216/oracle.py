import sys
sys.setrecursionlimit(20000)

def main():
    data = sys.stdin.read().split()
    n, B = map(int, data[:2])
    edges = list(map(int, data[2:]))

    # Build adjacency list
    adj = [[] for _ in range(n)]
    ptr = 0
    for _ in range(n-1):
        u = edges[ptr] - 1
        v = edges[ptr+1] - 1
        ptr += 2
        adj[u].append(v)
        adj[v].append(u)

    comp = [0]*n         # province index for each node
    capitals = []        # list of capitals (1-based)
    stack = []           # holds unassigned nodes during DFS

    # DFS returns how many nodes in this subtree are still unassigned
    def dfs(u, parent):
        cnt_here = 0
        # Process children first (post-order)
        for v in adj[u]:
            if v == parent:
                continue
            cnt_here += dfs(v, u)
            # If we reach B unassigned nodes, form a province here at u
            if cnt_here >= B:
                capitals.append(u+1)      # record capital as 1-based
                pid = len(capitals)       # new province index
                # Pop exactly cnt_here nodes from stack and assign them
                for _ in range(cnt_here):
                    x = stack.pop()
                    comp[x] = pid
                cnt_here = 0
        # After children, add u as unassigned
        stack.append(u)
        return cnt_here + 1

    # Run DFS from root 0
    dfs(0, -1)

    # If no province was formed, create one at node 1
    if not capitals:
        capitals.append(1)

    # Assign leftovers to last province
    last = len(capitals)
    while stack:
        comp[stack.pop()] = last

    # Print answer
    print(last)
    print(' '.join(map(str, comp)))
    print(' '.join(map(str, capitals)))

if __name__ == "__main__":
    main()
