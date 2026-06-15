import sys
import math

EPS = 1e-9

def y_at(seg, x):
    """Compute y-value of non-vertical segment at coordinate x."""
    (x1, y1), (x2, y2) = seg
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1)

def covers(seg, x):
    """Check if x lies within segment's x-range (inclusive with epsilon)."""
    (x1, _), (x2, _) = seg
    return x >= x1 - EPS and x <= x2 + EPS

def is_vertical(seg):
    """True if segment is vertical (x1 == x2 within epsilon)."""
    (x1, _), (x2, _) = seg
    return abs(x2 - x1) < EPS

def unique_sorted(xs):
    """Sort xs and remove near-duplicates using EPS tolerance."""
    xs.sort()
    res = []
    for v in xs:
        if not res or abs(v - res[-1]) >= EPS:
            res.append(v)
    return res

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))

    segments = []
    colors = []

    # Read segments, normalize so that x1 <= x2 (swap endpoints if needed)
    for _ in range(n):
        x1 = float(next(it)); y1 = float(next(it))
        x2 = float(next(it)); y2 = float(next(it))
        c = next(it)
        if x1 > x2:
            x1, x2 = x2, x1
            y1, y2 = y2, y1
        segments.append(((x1, y1), (x2, y2)))
        colors.append(c)

    xs = []

    # Add endpoints for non-vertical segments
    for seg in segments:
        if is_vertical(seg):
            continue
        (x1, _), (x2, _) = seg
        xs.append(x1)
        xs.append(x2)

    # Add intersection x-coordinates for each pair of non-vertical segments
    for i in range(n):
        seg_i = segments[i]
        if is_vertical(seg_i):
            continue
        (xi1, yi1), (xi2, yi2) = seg_i
        si = (yi2 - yi1) / (xi2 - xi1)          # slope
        ci = yi1 - si * xi1                     # intercept

        for j in range(i + 1, n):
            seg_j = segments[j]
            if is_vertical(seg_j):
                continue
            (xj1, yj1), (xj2, yj2) = seg_j
            sj = (yj2 - yj1) / (xj2 - xj1)
            cj = yj1 - sj * xj1

            # Parallel lines => no single intersection x
            if abs(si - sj) < 1e-12:
                continue

            # Compute intersection x of supporting lines
            x = (cj - ci) / (si - sj)

            # Only keep x if within both segments' x-overlap
            lo = max(xi1, xj1)
            hi = min(xi2, xj2)
            if x >= lo - EPS and x <= hi + EPS:
                xs.append(x)

    xs = unique_sorted(xs)

    sr = sg = sb = 0.0

    # Scan each interval between consecutive critical x's
    for k in range(len(xs) - 1):
        x_left = xs[k]
        x_right = xs[k + 1]
        mid = (x_left + x_right) / 2.0
        length = x_right - x_left

        best_y = 1e100
        best_c = None

        # Find segment with minimal y(mid) among those covering mid
        for seg, c in zip(segments, colors):
            if is_vertical(seg):
                continue
            if not covers(seg, mid):
                continue
            y = y_at(seg, mid)
            if y < best_y:
                best_y = y
                best_c = c

        # Add this interval to the winning color
        if best_c == 'R':
            sr += length
        elif best_c == 'G':
            sg += length
        elif best_c == 'B':
            sb += length
        # If best_c is None, no segment covers mid => contributes nothing

    # Print with 2 digits after decimal point (precision 0.01)
    print(f"R {sr:.2f}")
    print(f"G {sg:.2f}")
    print(f"B {sb:.2f}")

if __name__ == "__main__":
    main()
