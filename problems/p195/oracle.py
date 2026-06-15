import sys
sys.setrecursionlimit(10**7)
input = sys.stdin.readline

def main():
    n = int(input())
    # Build adjacency list; 0-based indexing
    children = [[] for _ in range(n)]
    parents = list(map(int, input().split()))
    for i, p in enumerate(parents, start=1):
        children[p-1].append(i)

    # dp0[u], dp1[u] as in the editorial
    dp0 = [0]*n
    dp1 = [0]*n

    # Post-order: since children always have larger index, loop u = n-1..0
    for u in range(n-1, -1, -1):
        # Case: u receives from its parent
        dp1[u] = 1 + sum(dp0[v] for v in children[u])

        # Sum of dp0 over all children
        sum0 = sum(dp0[v] for v in children[u])
        # If u matches no child
        best = sum0
        # If u matches exactly one child v
        for v in children[u]:
            cand = sum0 - dp0[v] + dp1[v]
            if cand > best:
                best = cand
        dp0[u] = best
        # Possibly treat u as a receiver if that’s equally good
        if dp1[u] < dp0[u]:
            dp1[u] = dp0[u]

    # Reconstruction of receivers
    take = [False]*n
    receivers = []
    for u in range(n):
        # If we previously marked take[u] but it wasn't needed, clear it
        if take[u] and dp1[u] == dp0[u]:
            take[u] = False

        if take[u]:
            # u receives
            receivers.append(u+1)   # store 1-based
            # its children cannot receive
            for v in children[u]:
                take[v] = False
        else:
            # u is free: find if u matched one child
            sum0 = sum(dp0[v] for v in children[u])
            picked = None
            for v in children[u]:
                if dp0[u] == sum0 - dp0[v] + dp1[v]:
                    picked = v
                    take[v] = True
                    break
            # all other children do not receive
            for v in children[u]:
                if v != picked:
                    take[v] = False

    # Output total money and sorted receivers
    total_money = dp0[0] * 1000
    print(total_money)
    print(*receivers)

if __name__ == "__main__":
    main()
