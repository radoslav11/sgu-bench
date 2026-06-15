import math
import sys


def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    N = int(next(it))
    u = float(next(it))
    T = float(next(it))

    w = [0.0] * N
    v = [0.0] * N
    for i in range(N):
        w[i] = float(next(it))
        v[i] = float(next(it))

    # Minimal time to cross all rivers: each river i needs at least w_i/u seconds,
    # because your east component in still water cannot exceed u.
    min_time = sum(wi / u for wi in w)
    if min_time > T + 1e-12:
        print(-1)
        return

    # Lambda must satisfy lambda > v_i + u for all i, otherwise t_i(lambda) is undefined.
    max_asympt = max(vi + u for vi in v)

    # Compute the vector of times t_i given lambda using the derived closed-form:
    # a = lambda - v_i
    # t_i = w_i * a / (u * sqrt(a^2 - u^2))
    def compute_times(lam: float):
        res = [0.0] * N
        uu = u * u
        for i in range(N):
            a = lam - v[i]
            # a must be > u, ensured by our binary search lower bound
            sqrt_term = math.sqrt(a * a - uu)
            res[i] = w[i] * a / (u * sqrt_term)
        return res

    # Binary search lambda so that sum t_i == T.
    low = max_asympt + 1e-9
    high = 1e18

    for _ in range(200):
        mid = (low + high) / 2.0
        ts = compute_times(mid)
        sumt = sum(ts)

        # sumt decreases as lambda increases.
        if sumt > T:
            low = mid
        else:
            high = mid

    lam = (low + high) / 2.0
    times = compute_times(lam)

    # Compute W (total east) and Y (total north), then distance.
    W = sum(w)
    Y = 0.0
    for i in range(N):
        ti = times[i]
        # north displacement in river i:
        # v_i * t_i + sqrt((u*t_i)^2 - w_i^2)
        Y += v[i] * ti + math.sqrt((u * ti) * (u * ti) - w[i] * w[i])

    dist = math.hypot(W, Y)  # sqrt(W^2 + Y^2)

    # Output formatting
    print(f"{dist:.10f}")
    print(" ".join(f"{ti:.10f}" for ti in times))


if __name__ == "__main__":
    solve()
