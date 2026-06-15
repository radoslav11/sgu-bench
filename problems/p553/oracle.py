import sys


def main():
    data = sys.stdin.buffer.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    k = int(next(it))
    w = [0] * n
    c = [0] * n
    for i in range(n):
        w[i] = int(next(it))
        c[i] = int(next(it))

    prefW = [0] * n
    prefC = [0] * n
    prefW[0] = w[0]
    prefC[0] = c[0]
    for i in range(1, n):
        prefW[i] = prefW[i - 1] + w[i]
        prefC[i] = prefC[i - 1] + c[i]

    def getW(l, r):
        if l > r:
            return 0
        return prefW[r] - (prefW[l - 1] if l > 0 else 0)

    def getC(l, r):
        if l > r:
            return 0
        return prefC[r] - (prefC[l - 1] if l > 0 else 0)

    best_total = 0
    best_x = 0
    best_y = 0

    scoreH = 0
    for x in range(0, n - m + 1):
        L = n - x
        if L < m:
            break

        W_hang = getW(L - m, L - 1)
        W_table = getW(0, L - m - 1)

        if W_hang > k * W_table:
            break

        lo, hi = 0, L - m
        best_mid = 0
        while lo <= hi:
            mid = (lo + hi) // 2
            if getW(mid, L - m - 1) * k >= W_hang:
                best_mid = mid
                lo = mid + 1
            else:
                hi = mid - 1
        y = best_mid

        total = scoreH + getC(0, y - 1)
        if total > best_total:
            best_total = total
            best_x = x
            best_y = y

        if x < n:
            scoreH += c[n - 1 - x]

    out = []
    if best_total == 0:
        out.append("0 0")
    else:
        p = best_x + best_y
        out.append(f"{p} {best_total}")
        out.append("H" * best_x + "T" * best_y)
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
