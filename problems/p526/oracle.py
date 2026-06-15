import sys
from bisect import bisect_left

# We will implement the same interval-layer propagation and reconstruction.
# Complexity: O(n^2) worst-case with n<=3000, should pass in PyPy with care,
# but C++ is safer. This Python is written efficiently (list operations).

def merge_open_blocked(intervals):
    """
    Merge blocked intervals representing OPEN interiors (x1,x2).
    We store as (l,r) boundaries but treat as open.
    Two open intervals (a,b) and (b,c) do NOT overlap, so we merge only if
    next.l < last.r (strict).
    """
    if not intervals:
        return []
    intervals.sort()
    merged = [list(intervals[0])]
    for l, r in intervals[1:]:
        if l < merged[-1][1]:
            if r > merged[-1][1]:
                merged[-1][1] = r
        else:
            merged.append([l, r])
    return [tuple(x) for x in merged]

def merge_closed(intervals):
    """
    Merge CLOSED reachable intervals [l,r].
    Closed intervals that touch should merge: next.l <= last.r
    """
    if not intervals:
        return []
    intervals.sort()
    merged = [list(intervals[0])]
    for l, r in intervals[1:]:
        if l <= merged[-1][1]:
            if r > merged[-1][1]:
                merged[-1][1] = r
        else:
            merged.append([l, r])
    return [tuple(x) for x in merged]

def subtract_open_from_closed(reach, blocked):
    """
    reach: list of closed intervals [l,r] (tuples)
    blocked: list of open intervals (a,b) stored as boundaries (a,b)
    Return closed intervals after removing interiors.
    Endpoints are safe.
    """
    res = []
    bi = 0
    nb = len(blocked)

    for l, r in reach:
        cl = l

        # advance blocked pointer while blocked ends at/before cl
        while bi < nb and blocked[bi][1] <= cl:
            bi += 1

        # process blocked intervals that start before r
        while bi < nb and blocked[bi][0] < r:
            a, b = blocked[bi]

            if b <= cl:
                bi += 1
                continue

            # keep [cl, a] (possibly point)
            if a >= cl:
                res.append((cl, a))

            # skip blocked interior by jumping to b (endpoint b is safe)
            cl = max(cl, b)

            if b <= r:
                bi += 1
            else:
                break

            if cl >= r:
                break

        # remaining tail
        if cl <= r:
            res.append((cl, r))

    return res

def clamp(x, a, b):
    if x < a:
        return a
    if x > b:
        return b
    return x

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    v = int(next(it))
    G = int(next(it))
    n = int(next(it))

    stones = []
    times = [0]
    for _ in range(n):
        x1 = int(next(it)); x2 = int(next(it)); t = int(next(it))
        stones.append((x1, x2, t))
        times.append(t)

    times = sorted(set(times))
    p = len(times)
    time_to_idx = {t:i for i,t in enumerate(times)}

    # blocked[i] = list of blocked open intervals at times[i]
    blocked = [[] for _ in range(p)]
    for x1, x2, t in stones:
        if x1 < x2:  # open interval empty if equal
            blocked[time_to_idx[t]].append((x1, x2))

    # merge blocked per layer with open-interval semantics
    for i in range(p):
        blocked[i] = merge_open_blocked(blocked[i])

    # layers[i] = reachable closed intervals at times[i]
    layers = [[] for _ in range(p)]
    cur = [(0, 0)]
    layers[0] = cur

    best_time = 1e100
    best_layer = -1
    best_x = 0.0

    def check(layer, intervals):
        nonlocal best_time, best_layer, best_x
        t0 = times[layer]
        # time window until next stone time (if any)
        dt = None if layer + 1 >= p else (times[layer+1] - t0)
        for l, r in intervals:
            if l <= G <= r:
                dist = 0
                x = float(G)
            elif G < l:
                dist = l - G
                x = float(l)
            else:
                dist = G - r
                x = float(r)

            arrival = t0 + dist / v

            valid = (dt is None) or (dist <= v * dt)
            if valid and arrival < best_time - 1e-12:
                best_time = arrival
                best_layer = layer
                best_x = x

    check(0, cur)

    # propagate
    for i in range(p - 1):
        D = v * (times[i+1] - times[i])

        expanded = [(l - D, r + D) for (l, r) in cur]
        merged = merge_closed(expanded)

        cur = subtract_open_from_closed(merged, blocked[i+1])
        layers[i+1] = cur

        check(i+1, cur)

    if best_layer == -1:
        sys.stdout.write("-1\n")
        return

    # reconstruct path positions at each layer time up to best_layer
    path = [0.0] * (best_layer + 1)
    path[best_layer] = best_x

    for i in range(best_layer - 1, -1, -1):
        D = v * (times[i+1] - times[i])
        target = path[i+1]

        # find any interval in layers[i] intersecting [target-D, target+D]
        chosen = None
        for l, r in layers[i]:
            cl = max(l, target - D)
            cr = min(r, target + D)
            if cl <= cr + 1e-9:
                chosen = clamp(target, cl, cr)
                break
        path[i] = chosen

    # build instructions: constant speed between consecutive stone times
    instr = []
    for i in range(best_layer):
        dt = times[i+1] - times[i]
        dx = path[i+1] - path[i]
        w = dx / dt  # within [-v, v]
        instr.append((w, float(dt)))

    # final run to Jasmine at speed +/-v
    final_dx = G - path[best_layer]
    if abs(final_dx) > 1e-12:
        w = float(v) if final_dx > 0 else float(-v)
        instr.append((w, abs(final_dx) / v))

    out = [str(len(instr))]
    for w, t in instr:
        out.append(f"{w:.15f} {t:.15f}")
    sys.stdout.write("\n".join(out) + "\n")

if __name__ == "__main__":
    solve()
