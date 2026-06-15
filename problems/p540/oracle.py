import sys, math


def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    s = int(next(it))
    vmin = int(next(it))
    vmax = int(next(it))

    x = [0] * n
    r = [0] * n
    g = [0] * n
    d = [0] * n
    for i in range(n):
        x[i] = int(next(it))
        r[i] = int(next(it))
        g[i] = int(next(it))
        d[i] = int(next(it))

    events = []

    for i in range(n):
        c = r[i] + g[i]

        k = 0
        while True:
            gs = d[i] - g[i] + k * c
            ge = d[i] + k * c

            if float(gs) > x[i] / vmin:
                break

            if ge <= 0:
                k += 1
                continue

            if x[i] / ge > vmax:
                k += 1
                continue

            t_lo = max(float(gs), x[i] / vmax)
            t_hi = min(float(ge), x[i] / vmin)

            if t_lo < 1e-15:
                t_lo = 1e-15

            if t_lo > t_hi + 1e-12:
                k += 1
                continue

            vl = x[i] / t_hi
            vh = x[i] / t_lo

            if vl < vmin:
                vl = float(vmin)
            if vh > vmax:
                vh = float(vmax)

            if vl > vh + 1e-12:
                k += 1
                continue

            events.append((vl, +1))
            events.append((vh, -1))

            k += 1

    events.sort(key=lambda p: (p[0], -p[1]))

    cnt = 0
    best_cnt = 0
    best_v = float(vmax)

    i = 0
    m = len(events)
    while i < m:
        pos = events[i][0]

        while i < m and events[i][0] == pos and events[i][1] > 0:
            cnt += 1
            i += 1

        if cnt > best_cnt or (cnt == best_cnt and pos > best_v):
            best_cnt = cnt
            best_v = pos

        while i < m and events[i][0] == pos and events[i][1] < 0:
            cnt -= 1
            i += 1

    eps = 1e-9
    switched = []
    for i in range(n):
        t = x[i] / best_v
        c = r[i] + g[i]

        rem = math.fmod(t - d[i] + g[i], c)
        if rem < -eps:
            rem += c

        if rem > g[i] + eps and rem < c - eps:
            switched.append(i + 1)

    sys.stdout.write(f"{best_v:.10f}\n")
    sys.stdout.write(f"{len(switched)}\n")
    if switched:
        sys.stdout.write(" ".join(map(str, switched)) + "\n")


if __name__ == "__main__":
    solve()
