import sys


def solve():
    data = list(map(int, sys.stdin.read().split()))
    n = data[0]
    a = data[1:1 + n]

    for x in a:
        if x < 0 or x > n - 1:
            print("NO")
            return

    picked = []
    used = [False] * (n + 1)
    larger_picked = [0] * (n + 1)
    smaller_pos = [[] for _ in range(n + 1)]
    g_values = [0] * n
    g_prev = 0

    for k in range(n):
        best = -1
        best_g = 10**18

        for v in range(1, n + 1):
            if used[v]:
                continue

            need = a[v - 1] - larger_picked[v]
            total_smaller = len(smaller_pos[v])
            if need < 0 or need > total_smaller:
                continue

            if need == 0:
                g_lo = 0
            else:
                g_lo = smaller_pos[v][need - 1] + 1

            if need == total_smaller:
                g_hi = k
            else:
                g_hi = smaller_pos[v][need]

            cur_g = max(g_lo, g_prev)
            if cur_g > g_hi:
                continue

            if cur_g < best_g:
                best = v
                best_g = cur_g

        if best == -1:
            print("NO")
            return

        picked.append(best)
        used[best] = True
        g_values[k] = best_g
        g_prev = best_g

        for v in range(1, n + 1):
            if v < best:
                larger_picked[v] += 1
            elif v > best:
                smaller_pos[v].append(k)

    pi = [0] * (n + 1)
    f = [0] * (n + 1)
    for r in range(1, n + 1):
        pi[r] = picked[n - r]
        f[r] = n - g_values[n - r]

    seq = []
    next_primed = 1
    for r in range(1, n + 1):
        while next_primed <= f[r]:
            seq.append(-pi[next_primed])
            next_primed += 1
        seq.append(pi[r])

    print("YES")
    print(*seq)


if __name__ == "__main__":
    solve()
