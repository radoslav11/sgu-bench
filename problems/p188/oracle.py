import sys


def first_c_geq(val):
    # smallest integer c such that 1000*c >= val
    return -((-val) // 1000)


def last_c_leq(val):
    # largest integer c such that 1000*c <= val
    return val // 1000


def count_meetings(l1, v1, l2, v2, T):
    if (v1 > 0) == (v2 > 0):
        return 0

    dv = v1 - v2
    dl = l2 - l1

    if dv > 0:
        c_low = first_c_geq(-dl + 1)
        c_high = last_c_leq(dv * T - dl)
    else:
        c_low = first_c_geq(dv * T - dl)
        c_high = last_c_leq(-dl - 1)

    if c_high < c_low:
        return 0
    return c_high - c_low + 1


def solve():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    T = int(data[1])

    L = list(map(int, data[2:2 + n]))
    V = list(map(int, data[2 + n:2 + 2 * n]))

    ans = [0] * n

    for i in range(n):
        total = 0
        for j in range(n):
            if i == j:
                continue
            total += count_meetings(L[i], V[i], L[j], V[j], T)
        ans[i] = total

    sys.stdout.write(" ".join(map(str, ans)) + "\n")


if __name__ == "__main__":
    solve()
