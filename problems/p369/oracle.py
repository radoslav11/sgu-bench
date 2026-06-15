import sys
sys.setrecursionlimit(10**7)
input = sys.stdin.readline

def main():
    K = int(input())
    xs = [0]*K
    ys = [0]*K
    # Map each x (resp. y) to the list of point indices having that coordinate
    map_x = {}
    map_y = {}

    for i in range(K):
        xi, yi = map(int, input().split())
        xs[i] = xi
        ys[i] = yi
        map_x.setdefault(xi, []).append(i)
        map_y.setdefault(yi, []).append(i)

    visited = [False]*K
    answer = 0

    # BFS/DFS will consume each group once; after exploring all points
    # on some x or y, we clear its list to avoid revisiting.
    from collections import deque
    for i in range(K):
        if not visited[i]:
            queue = deque([i])
            visited[i] = True
            comp_rows = set()
            comp_cols = set()

            while queue:
                u = queue.popleft()
                rx, ry = xs[u], ys[u]
                comp_rows.add(rx)
                comp_cols.add(ry)

                # Explore all other points sharing the same x
                for v in map_x.get(rx, []):
                    if not visited[v]:
                        visited[v] = True
                        queue.append(v)
                # Clear so we don't re-explore this row later
                map_x[rx] = []

                # Explore all other points sharing the same y
                for v in map_y.get(ry, []):
                    if not visited[v]:
                        visited[v] = True
                        queue.append(v)
                # Clear so we don't re-explore this column later
                map_y[ry] = []

            # This component can be filled to (#rows)×(#cols)
            answer += len(comp_rows) * len(comp_cols)

    print(answer)

if __name__ == "__main__":
    main()
