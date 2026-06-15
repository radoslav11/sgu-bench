import sys
from math import comb

EPS = 1e-15


def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    l1 = int(data[1])
    l2 = int(data[2])
    p = float(data[3]) / 100.0

    weight = [0.0] * (n + 1)
    total = 0.0
    for k in range(n + 1):
        if l1 <= k and l2 <= (n - k):
            w = comb(k, l1) * comb(n - k, l2)
        else:
            w = 0
        weight[k] = float(w)
        total += weight[k]

    post = [w / total for w in weight]

    pref = [0.0] * (n + 2)
    for k in range(n + 1):
        pref[k + 1] = pref[k] + post[k]

    best_len = n + 1
    best_a, best_b = 0, n
    for length in range(n + 1):
        for a in range(0, n - length + 1):
            b = a + length
            prob = pref[b + 1] - pref[a]
            if prob + EPS >= p:
                if length < best_len or (length == best_len and a < best_a):
                    best_len = length
                    best_a, best_b = a, b

    print(best_a, best_b)


if __name__ == "__main__":
    solve()
