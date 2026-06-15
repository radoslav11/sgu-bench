import sys
import threading


def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    A = float(next(it))
    B = float(next(it))
    pts = []
    ans = 0.0

    for _ in range(N):
        a = float(next(it))
        b = float(next(it))
        c = float(next(it))
        x = a / c
        y = b / c
        pts.append((x, y))
        M = min(A / x, B / y)
        if M > ans:
            ans = M

    pts = sorted(set(pts))
    if len(pts) > 1:

        def cross(o, a, b):
            return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (
                b[0] - o[0]
            )

        lower = []
        for p in pts:
            while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= 0:
                lower.pop()
            lower.append(p)
        upper = []
        for p in reversed(pts):
            while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= 0:
                upper.pop()
            upper.append(p)
        hull = lower[:-1] + upper[:-1]
    else:
        hull = pts

    def intersect(o, t, p, q):
        otx, oty = t[0] - o[0], t[1] - o[1]
        pqx, pqy = q[0] - p[0], q[1] - p[1]
        num = (p[0] - o[0]) * pqy - (p[1] - o[1]) * pqx
        den = otx * pqy - oty * pqx
        if abs(den) < 1e-12:
            return None
        alpha = num / den
        return (o[0] + alpha * otx, o[1] + alpha * oty)

    def on_seg(p, q, r):
        return (
            min(p[0], q[0]) - 1e-9 <= r[0] <= max(p[0], q[0]) + 1e-9
            and min(p[1], q[1]) - 1e-9 <= r[1] <= max(p[1], q[1]) + 1e-9
        )

    O = (0.0, 0.0)
    T = (A, B)
    Hn = len(hull)
    for i in range(Hn):
        p1 = hull[i]
        p2 = hull[(i + 1) % Hn]
        if (
            abs(
                (T[0] - O[0]) * (p1[1] - p2[1])
                - (T[1] - O[1]) * (p1[0] - p2[0])
            )
            < 1e-12
        ):
            continue
        I = intersect(O, T, p1, p2)
        if I and on_seg(p1, p2, I):
            M = min(A / I[0], B / I[1])
            if M > ans:
                ans = M

    print(f"{ans:.6f}")


if __name__ == "__main__":
    threading.Thread(target=main).start()
