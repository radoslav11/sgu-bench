import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])

    # Number of circle lines is fixed by cycle rank:
    k = m - n + 1
    if k < 1:
        sys.stdout.write("No solution\n")
        return

    # s must satisfy:
    # 3k <= s <= 10k
    # ceil((n + 3k - 3)/2) <= s <= n + k - 1
    s_min = max(3 * k, (n + 3 * k - 2) // 2)   # ceil((x)/2) with x = n+3k-3
    s_max = min(10 * k, n + k - 1)

    if s_min > s_max:
        sys.stdout.write("No solution\n")
        return

    s = s_min

    # Decide individual circle sizes: start at 3 and distribute extra up to 10.
    sizes = [3] * k
    extra = s - 3 * k
    i = 0
    while i < k and extra > 0:
        add = min(extra, 7)   # 3 + 7 = 10 max
        sizes[i] += add
        extra -= add
        i += 1

    # Build circles with station IDs:
    circles = [[] for _ in range(k)]
    next_id = 1
    prev_shared = None  # transfer station from previous circle (if any)

    for i in range(k):
        c = circles[i]

        # Reuse previous shared station as the first station of this circle
        if prev_shared is not None:
            c.append(prev_shared)

        # Fill remaining positions with fresh station numbers
        while len(c) < sizes[i]:
            c.append(next_id)
            next_id += 1

        # The last station becomes shared with the next circle (if any)
        if i + 1 < k:
            prev_shared = c[-1]

    # Collect all non-transfer stations as possible radial attachment points.
    attach = []
    for i in range(k):
        sz = sizes[i]
        for j in range(sz):
            # Transfer if it's shared with previous (first) or next (last)
            is_transfer = (i > 0 and j == 0) or (i + 1 < k and j == sz - 1)
            if not is_transfer:
                attach.append(circles[i][j])

    # Remaining station IDs [next_id..n] are radial terminals.
    radials = []
    idx = 0
    for t in range(next_id, n + 1):
        # Connect attach[idx] -- t
        radials.append((attach[idx], t))
        idx += 1

    # Output in required format
    out_lines = []
    out_lines.append(str(k))
    for i in range(k):
        out_lines.append(str(sizes[i]) + " " + " ".join(map(str, circles[i])))
    out_lines.append(str(len(radials)))
    out_lines.extend(f"{a} {b}" for a, b in radials)

    sys.stdout.write("\n".join(out_lines) + "\n")


if __name__ == "__main__":
    solve()
