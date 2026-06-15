import sys
from bisect import bisect_left
from collections import defaultdict


def check_one_step(p, q):
    n = len(p)
    used = [False] * n
    swaps = []

    for i in range(n):
        if used[i] or p[i] == q[i]:
            continue

        found = -1
        for j in range(n):
            if not used[j] and p[i] == q[j] and p[j] == q[i]:
                found = j
                break

        if found == -1:
            return False, []

        used[i] = True
        used[found] = True
        swaps.append((i, found))

    return True, swaps


def solve():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    p = list(map(int, data[1:]))

    q = sorted(p)

    if p == q:
        print(0)
        return

    ok, swaps = check_one_step(p, q)
    if ok:
        print(1)
        out = [str(len(swaps))]
        for i, j in swaps:
            out.append(str(i + 1))
            out.append(str(j + 1))
        print(" ".join(out))
        return

    cnt = defaultdict(int)
    perm = [0] * n
    for i, val in enumerate(p):
        base = bisect_left(q, val)
        perm[i] = base + cnt[val]
        cnt[val] += 1

    turns = [[], []]
    vis = [False] * n

    for i in range(n):
        if vis[i]:
            continue

        cycle = []
        u = i
        while not vis[u]:
            vis[u] = True
            cycle.append(u)
            u = perm[u]

        k = len(cycle)
        if k == 1:
            continue
        if k == 2:
            turns[0].append((cycle[0], cycle[1]))
            continue

        l = 0
        r = k - 2
        while l < r:
            turns[0].append((cycle[l], cycle[r]))
            turns[1].append((cycle[l], cycle[r + 1]))
            l += 1
            r -= 1
        if l == r:
            turns[1].append((cycle[l], cycle[r + 1]))

    print(2)
    for t in range(2):
        out = [str(len(turns[t]))]
        for i, j in turns[t]:
            out.append(str(i + 1))
            out.append(str(j + 1))
        print(" ".join(out))


if __name__ == "__main__":
    solve()
