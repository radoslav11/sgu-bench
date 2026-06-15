import sys


def lt_slope(p1, q1, p2, q2):
    return p1 * q2 < p2 * q1


def solve_case(buildings):
    buildings.sort(key=lambda x: x[0])
    left_x = buildings[0][0]
    right_x = buildings[-1][1]
    a = [(left_x - 2, left_x - 1, 0)] + buildings + [(right_x + 1, right_x + 2, 0)]
    last = len(a) - 1
    lit_left = [False] * (last + 1)
    lit_right = [False] * (last + 1)

    def cover_left(px, py):
        bn, bd = 0, 1
        for j in range(last):
            l, r, h = a[j]
            if r <= px:
                continue
            if not lt_slope(l - px, py, bn, bd):
                lit_left[j] = True
            if h >= py:
                break
            if lt_slope(bn, bd, r - px, py - h):
                bn, bd = r - px, py - h

    def cover_right(px, py):
        bn, bd = 0, 1
        for j in range(last - 1, 0, -1):
            l, r, h = a[j]
            if l >= px:
                continue
            if not lt_slope(px - r, py, bn, bd):
                lit_right[j] = True
            if h >= py:
                break
            if lt_slope(bn, bd, px - l, py - h):
                bn, bd = px - l, py - h

    ans = 0

    def place(px, py):
        nonlocal ans
        ans += 1
        cover_left(px, py)
        cover_right(px, py)

    for i in range(last):
        if not lit_left[i + 1]:
            if a[i][2] < a[i + 1][2]:
                place(a[i + 1][0], a[i + 1][2])
            else:
                place(a[i][1], a[i][2])
            continue

        if not lit_right[i]:
            best = 0
            bn, bd = 1, 0
            for j in range(i + 1, last):
                dist = a[j][0] - a[i][1]
                height = a[j][2]
                if not lt_slope(bn, bd, dist, height):
                    best = j
                    bn, bd = dist, height
            place(a[best][0], a[best][2])

    return ans


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    idx = 0
    t = data[idx]
    idx += 1
    out = []
    for _ in range(t):
        n = data[idx]
        idx += 1
        buildings = []
        for _ in range(n):
            l, r, h = data[idx], data[idx + 1], data[idx + 2]
            idx += 3
            buildings.append((l, r, h))
        out.append(str(solve_case(buildings)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
