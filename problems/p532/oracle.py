import sys

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))

    # Store normalized segments:
    # horizontals: (y, x_left, x_right)
    # verticals:   (x, y_low, y_high)
    horizontals = []
    verticals = []

    for _ in range(n):
        x1 = int(next(it)); y1 = int(next(it))
        x2 = int(next(it)); y2 = int(next(it))

        if y1 == y2:
            # Horizontal segment: normalize x1 <= x2
            if x1 > x2:
                x1, x2 = x2, x1
            horizontals.append((y1, x1, x2))
        else:
            # Vertical segment: normalize y1 <= y2
            if y1 > y2:
                y1, y2 = y2, y1
            verticals.append((x1, y1, y2))

    ans = 0

    # Enumerate all pairs of horizontal segments
    H = len(horizontals)
    for i in range(H):
        y1, a1, b1 = horizontals[i]
        for j in range(i + 1, H):
            y2, a2, b2 = horizontals[j]

            # Compute the x-overlap interval where a vertical can hit both
            x_left = max(a1, a2)
            x_right = min(b1, b2)

            # Need positive width possibility; if no overlap, skip
            if x_left >= x_right:
                continue

            # Vertical must span between these y's
            y_min = y1 if y1 < y2 else y2
            y_max = y2 if y1 < y2 else y1

            cnt = 0
            # Count vertical segments that can serve as left/right sides
            for vx, vy_low, vy_high in verticals:
                if (x_left <= vx <= x_right) and (vy_low <= y_min) and (vy_high >= y_max):
                    cnt += 1

            # Add number of ways to choose two verticals
            ans += cnt * (cnt - 1) // 2

    sys.stdout.write(str(ans))

if __name__ == "__main__":
    solve()
