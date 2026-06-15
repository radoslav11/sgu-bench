import sys
import threading


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    k = int(next(it))
    poly = [(int(next(it)), int(next(it))) for _ in range(n)]
    queries = [(int(next(it)), int(next(it))) for _ in range(m)]

    idx = min(range(n), key=lambda i: (poly[i][0], poly[i][1]))
    pts = poly[idx:] + poly[:idx]
    base = pts[0]
    fan = pts[1:]

    def cross(a, b, c):
        return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0])

    def orient(a, b, c):
        v = cross(a, b, c)
        if v > 0:
            return 1
        if v < 0:
            return -1
        return 0

    def in_triangle(a, b, c, p):
        d1 = orient(a, b, p)
        d2 = orient(b, c, p)
        d3 = orient(c, a, p)
        return (d1 >= 0 and d2 >= 0 and d3 >= 0) or (
            d1 <= 0 and d2 <= 0 and d3 <= 0
        )

    cnt = 0
    for q in queries:
        if orient(base, fan[0], q) < 0:
            continue
        if orient(base, fan[-1], q) > 0:
            continue
        L, R = 0, len(fan) - 1
        while R - L > 1:
            mid = (L + R) // 2
            if orient(base, fan[mid], q) >= 0:
                L = mid
            else:
                R = mid
        if in_triangle(base, fan[L], fan[R], q):
            cnt += 1

    sys.stdout.write("YES\n" if cnt >= k else "NO\n")


if __name__ == "__main__":
    threading.Thread(target=main).start()
