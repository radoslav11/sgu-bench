import sys

def compute_cost_for_d(x, d):
    b = [xi - d * i for i, xi in enumerate(x)]
    b.sort()
    n = len(b)
    L = b[n // 2]
    cost = sum(abs(bi - L) for bi in b)
    return cost, L

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    x = list(map(int, data[1:1 + n]))

    lo, hi = 0.0, 1e7
    best_d = 0.0
    best_cost = float('inf')
    best_L = 0.0

    for _ in range(200):
        m1 = lo + (hi - lo) / 3
        m2 = hi - (hi - lo) / 3
        cost1, L1 = compute_cost_for_d(x, m1)
        cost2, L2 = compute_cost_for_d(x, m2)
        if cost1 < cost2:
            hi = m2
            if cost1 < best_cost:
                best_cost, best_d, best_L = cost1, m1, L1
        else:
            lo = m1
            if cost2 < best_cost:
                best_cost, best_d, best_L = cost2, m2, L2

    best_cost, best_L = compute_cost_for_d(x, best_d)
    print(f"{best_cost:.10f}")
    y = [best_L + best_d * i for i in range(n)]
    print(" ".join(f"{yi:.10f}" for yi in y))

if __name__ == "__main__":
    main()
