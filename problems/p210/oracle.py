import sys
def hungarian(cost):
    # cost is an n×n matrix, 0-based
    n = len(cost)
    INF = 10**30
    # We use 1-based indexing internally
    u = [0]*(n+1)
    v = [0]*(n+1)
    p = [0]*(n+1)
    way = [0]*(n+1)

    for i in range(1, n+1):
        p[0] = i
        j0 = 0
        minv = [INF]*(n+1)
        used = [False]*(n+1)
        while True:
            used[j0] = True
            i0 = p[j0]
            delta = INF
            j1 = 0
            # try to improve all columns not yet used
            for j in range(1, n+1):
                if not used[j]:
                    cur = cost[i0-1][j-1] - u[i0] - v[j]
                    if cur < minv[j]:
                        minv[j] = cur
                        way[j] = j0
                    if minv[j] < delta:
                        delta = minv[j]
                        j1 = j
            # update potentials
            for j in range(n+1):
                if used[j]:
                    u[p[j]] += delta
                    v[j]   -= delta
                else:
                    minv[j] -= delta
            j0 = j1
            # found free column?
            if p[j0] == 0:
                break

        # augmenting
        while True:
            j1 = way[j0]
            p[j0] = p[j1]
            j0 = j1
            if j0 == 0:
                break

    # build assignment: for each column j, row = p[j]
    assign = [0]*n
    for j in range(1, n+1):
        assign[j-1] = p[j]-1
    # total cost can be computed if needed
    # cost_value = sum(cost[assign[j]][j] for j in range(n))
    return assign

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    A = [int(next(it)) for _ in range(n)]
    likes = []
    for _ in range(n):
        k = int(next(it))
        arr = [int(next(it)) - 1 for __ in range(k)]
        likes.append(arr)

    # build cost matrix: rows=girls, cols=sons
    # Windows where son s likes girl g get cost = -A[s]^2
    cost = [[0]*n for _ in range(n)]
    for s in range(n):
        w = A[s]*A[s]
        for g in likes[s]:
            cost[g][s] = -w

    assign = hungarian(cost)

    # for each son s, if assigned girl is in his list, print it+1; else 0
    # convert each likes[s] to a set for O(1) checks
    lsets = [set(lst) for lst in likes]
    out = []
    for s in range(n):
        g = assign[s]
        if g in lsets[s]:
            out.append(str(g+1))
        else:
            out.append('0')
    sys.stdout.write(" ".join(out))

if __name__ == "__main__":
    main()
