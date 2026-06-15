"""Tests for p553 (Sultan's Pearls).

- 2 <= n <= 2e5, 1 <= m < n, 1 <= k <= 10, 1 <= w_i, c_i <= 1000.
- Initial configuration MUST be motionless: sum of the m hanging weights
  <= k * sum of the table weights. The helper below enforces this by
  rejecting and reshaping any draw that starts off balance.

Seed plan:

- seed 1: minimal input (n = 2, m = 1).

- seed 2: maximal n with k = 1 (tightest friction) and a weight profile
  that lets many H/T moves stay safe -- a long feasible run.

- seed 3: maximal n with k = 10 (loosest friction) and heavier hanging
  pearls, a different balance regime from seed 2.

- seeds 4+: random sizes, small/large m, adversarial weight layouts
  (uniform weights, heavy tail near the hanging end, near-critical initial
  balance where almost no move is safe).
"""

import random
import sys

WMAX = 1000
CMAX = 1000
NMAX = 200000


def motionless(w, m, k):
    n = len(w)
    hang = sum(w[n - m:])
    table = sum(w[: n - m])
    return hang <= k * table


def emit(w, c, m, k):
    n = len(w)
    out = [f"{n} {m} {k}"]
    for i in range(n):
        out.append(f"{w[i]} {c[i]}")
    sys.stdout.write("\n".join(out) + "\n")


def build_balanced(rng, n, m, k, w_lo=1, w_hi=WMAX, c_lo=1, c_hi=CMAX,
                   hang_scale=1.0):
    """Generate weights/costs; force the initial state to be motionless.

    Table pearls draw from [w_lo, w_hi] but are bumped up if needed so that
    the friction budget k * (table weight) can cover at least one unit per
    hanging pearl. Hanging pearls then draw from a range that keeps the
    running total within that budget.
    """
    table_n = n - m
    w = [rng.randint(w_lo, w_hi) for _ in range(table_n)]
    # Guarantee feasibility: need k * sum(table) >= m (each hang pearl >=1).
    while k * sum(w) < m:
        # Raise random table pearls to the maximum to grow the budget.
        i = rng.randrange(table_n)
        w[i] = w_hi
        if sum(1 for x in w if x == w_hi) == table_n and k * sum(w) < m:
            # Even all-max table cannot balance; impossible for these
            # (n, m, k). Caller must avoid this regime.
            raise ValueError("infeasible (n, m, k) for a motionless start")

    budget = k * sum(w)
    remaining = budget
    hang = []
    for j in range(m):
        left = m - j
        # Keep at least 1 unit reserved for each remaining hanging pearl.
        cap = remaining - (left - 1)
        hi = min(w_hi, cap)
        if hang_scale < 1.0:
            hi = max(1, min(hi, int(w_hi * hang_scale)))
        hi = max(1, hi)
        val = rng.randint(1, hi)
        hang.append(val)
        remaining -= val
    w.extend(hang)
    c = [rng.randint(c_lo, c_hi) for _ in range(n)]
    assert motionless(w, m, k), "internal: generated unbalanced start"
    return w, c


def main():
    seed = int(sys.argv[1])
    rng = random.Random(seed)

    if seed == 1:
        # n=2, m=1: take is impossible (table empties immediately) but the
        # start is motionless by construction (hang <= k*table).
        emit([2, 1], [5, 9], 1, 1)
        return

    if seed == 2:
        n = NMAX
        m = n // 2
        k = 1
        w, c = build_balanced(rng, n, m, k, w_lo=1, w_hi=1000)
        emit(w, c, m, k)
        return

    if seed == 3:
        n = NMAX
        m = n // 3
        k = 10
        w, c = build_balanced(rng, n, m, k, w_lo=1, w_hi=1000)
        emit(w, c, m, k)
        return

    if seed == 4:
        # Max n, small m -> hanging part tiny, many T moves possible.
        n = NMAX
        m = rng.randint(1, 5)
        k = rng.randint(1, 10)
        w, c = build_balanced(rng, n, m, k)
        emit(w, c, m, k)
        return

    if seed == 5:
        # Max n, large m (most pearls hang) -> small table. Table must be
        # heavy enough that k * sum(table) >= m, so keep a sizeable table.
        n = NMAX
        table_n = max(1, n // 20)
        m = n - table_n
        k = 10
        w, c = build_balanced(rng, n, m, k, w_hi=1000, hang_scale=0.3)
        emit(w, c, m, k)
        return

    if seed == 6:
        # Uniform weights everywhere; balance hinges purely on counts.
        n = NMAX
        m = rng.randint(1, n - 1)
        # Need m <= k*(n-m) for uniform weight to be motionless.
        k = 10
        while m > k * (n - m):
            m = rng.randint(1, n - 1)
        wval = rng.randint(1, 1000)
        w = [wval] * n
        c = [rng.randint(1, CMAX) for _ in range(n)]
        emit(w, c, m, k)
        return

    if seed == 7:
        # Near-critical start: hanging weight as large as friction allows.
        n = NMAX
        m = n // 4
        k = rng.randint(1, 10)
        table_n = n - m
        w = [rng.randint(1, 1000) for _ in range(table_n)]
        budget = k * sum(w)
        # Distribute the budget tightly among hanging pearls.
        hang = [1] * m
        remaining = budget - m
        idx = 0
        while remaining > 0 and idx < m:
            add = min(999, remaining)
            hang[idx] += add
            remaining -= add
            idx += 1
        w.extend(hang)
        c = [rng.randint(1, CMAX) for _ in range(n)]
        assert motionless(w, m, k)
        emit(w, c, m, k)
        return

    # Random mid/large cases.
    n = rng.randint(2, NMAX)
    k = rng.randint(1, 10)
    scale = rng.choice([1.0, 0.5, 0.2])
    # Retry m until a motionless start is achievable for this (n, k).
    for _ in range(50):
        m = rng.randint(1, n - 1)
        try:
            w, c = build_balanced(rng, n, m, k, hang_scale=scale)
            break
        except ValueError:
            continue
    else:
        m = 1
        w, c = build_balanced(rng, n, m, k, hang_scale=scale)
    emit(w, c, m, k)


if __name__ == "__main__":
    main()
