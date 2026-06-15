import sys
from collections import deque

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))

    # Build adjacency list with constraints.
    # For each edge (a,b) with endpoint signs d1,d2, define w = d1*d2 in {-1,+1}.
    # Constraint:
    #   w = +1 => x_a != x_b
    #   w = -1 => x_a == x_b
    adj = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(next(it))
        b = int(next(it))
        d1 = int(next(it))
        d2 = int(next(it))
        w = d1 * d2
        adj[a].append((b, w))
        adj[b].append((a, w))

    # color[v] = assigned value of x_v (0/1), -1 means unassigned/unvisited
    color = [-1] * (n + 1)

    flips = []      # vertices where x_v = 1 in our chosen optimal solution

    for start in range(1, n + 1):
        if color[start] != -1:
            continue

        # Start BFS for this connected component.
        color[start] = 0
        groups = [[], []]         # groups[0] and groups[1] store vertices by color
        groups[0].append(start)

        q = deque([start])
        possible = True

        while q and possible:
            u = q.popleft()
            cu = color[u]

            for v, w in adj[u]:
                # Required color for v given u:
                # if w==+1 => different: need = 1 - cu
                # if w==-1 => same:     need = cu
                need = (1 - cu) if w == 1 else cu

                if color[v] == -1:
                    color[v] = need
                    groups[need].append(v)
                    q.append(v)
                elif color[v] != need:
                    possible = False

        if not possible:
            sys.stdout.write("NO\n")
            return

        # To minimize flips in this component, pick the smaller of the two groups
        # to be x_v = 1 (i.e., vertices we will flip).
        smaller = groups[0] if len(groups[0]) <= len(groups[1]) else groups[1]
        flips.extend(smaller)

    flips.sort()
    out = ["YES", str(len(flips))]
    out.extend(str(v) for v in flips)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
