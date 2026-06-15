"""Tests for p516 (Schedule): N, M <= 1e5, 0 <= T <= 123 non-working hours.

All dates lie in 2009 (a non-leap year). Constraints enforced by the
generator: the N schedule start times D_i are strictly ascending with
D_1 <= T1; every update has A_i <= B_i < E_i and the A_i are non-descending;
T1 < T2. Guardian names are short alphabetic strings drawn from a small pool
so the same person accumulates non-working hours across intervals.

Seed 1 is the minimal instance. Seeds 2 and 3 are maximal (N = M = 1e5) with
different structure: seed 2 spreads duties over the whole year with many
guardians, seed 3 packs everything into a few busy weeks with few guardians
(stressing weekly non-working accumulation).
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

# Minutes in the year 2009 (non-leap). Domain [0, YEAR_MIN).
CUM = [0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334]
DAYS_IN_MONTH = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
YEAR_MIN = 365 * 1440  # last valid minute is YEAR_MIN-1 = 2009-12-31 23:59


def fmt(minute):
    minute = max(0, min(YEAR_MIN - 1, minute))
    day_of_year = minute // 1440
    rem = minute % 1440
    hh = rem // 60
    mm = rem % 60
    mon = 0
    d = day_of_year
    while mon < 11 and d >= DAYS_IN_MONTH[mon]:
        d -= DAYS_IN_MONTH[mon]
        mon += 1
    return f"2009-{mon + 1:02d}-{d + 1:02d} {hh:02d}:{mm:02d}"


def rand_minute(lo, hi):
    return rng.randint(lo, hi)


NAME_POOL = [
    "Vasya", "Vanya", "Petya", "Kolya", "Misha", "Dima",
    "Sasha", "Grisha", "Lesha", "Tolya",
]


def gen(n, m, t, name_count, span_lo, span_hi):
    names = NAME_POOL[:name_count] if name_count <= len(NAME_POOL) else (
        NAME_POOL + [f"G{i}" for i in range(name_count - len(NAME_POOL))]
    )

    # Observation window [T1, T2).
    t1 = rand_minute(span_lo + 1, (span_lo + span_hi) // 2)
    t2 = rand_minute(t1 + 1, span_hi)

    # N schedule start times strictly ascending, with D_1 <= T1.
    d1 = rand_minute(span_lo, t1)
    starts = {d1}
    while len(starts) < n:
        starts.add(rand_minute(span_lo, span_hi))
    starts = sorted(starts)
    # Re-clamp the first to satisfy D_1 <= T1.
    if starts[0] > t1:
        starts[0] = d1
        starts = sorted(set(starts))
    while len(starts) < n:
        cand = rand_minute(span_lo, span_hi)
        if cand not in starts:
            starts.append(cand)
            starts.sort()
    starts = starts[:n]
    if starts[0] > t1:
        starts[0] = d1
    starts.sort()

    sched = [(s, rng.choice(names)) for s in starts]

    # M updates with non-descending A_i and A_i <= B_i < E_i.
    updates = []
    a_times = sorted(rand_minute(span_lo, span_hi - 2) for _ in range(m))
    for a in a_times:
        b = rand_minute(a, min(span_hi - 1, a + rng.randint(0, 5000)))
        e = rand_minute(b + 1, min(span_hi, b + rng.randint(1, 5000)))
        updates.append((a, b, e, rng.choice(names)))

    out = [f"{n} {m} {t}", f"{fmt(t1)} {fmt(t2)}"]
    for s, name in sched:
        out.append(f"{fmt(s)} {name}")
    for a, b, e, name in updates:
        out.append(f"{fmt(a)} {fmt(b)} {fmt(e)} {name}")
    print("\n".join(out))


if seed == 1:
    # Minimal: 1 schedule entry, 0 updates, observe a small window.
    print("1 0 2")
    print("2009-01-05 09:00 2009-01-12 09:00")
    print("2009-01-01 00:00 Vasya")
elif seed == 2:
    gen(10**5, 10**5, 60, 10, 0, YEAR_MIN - 1)
elif seed == 3:
    # Few guardians, duties packed into the first ~10 weeks, low T -> many
    # invalid stretches and heavy weekly accumulation.
    gen(10**5, 10**5, 5, 2, 0, YEAR_MIN - 1)
elif seed == 4:
    gen(1000, 1000, 0, 3, 0, YEAR_MIN - 1)
elif seed == 5:
    gen(500, 2000, 123, 5, 0, YEAR_MIN - 1)
else:
    n = rng.randint(1, 3000)
    m = rng.randint(0, 3000)
    t = rng.randint(0, 123)
    nc = rng.randint(1, 8)
    span = rng.choice([7 * 1440, 30 * 1440, YEAR_MIN - 1])
    gen(n, m, t, nc, 0, span)
