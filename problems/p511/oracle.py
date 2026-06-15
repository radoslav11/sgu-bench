import sys
from math import gcd


def mod_inverse(a, m):
    if m == 1:
        return 0
    old_r, r = a, m
    old_s, s = 1, 0
    while r:
        q = old_r // r
        old_r, r = r, old_r - q * r
        old_s, s = s, old_s - q * s
    return old_s % m


def primitive_root(p):
    if p == 2:
        return 1
    phi = p - 1
    x = phi
    factors = []
    d = 2
    while d * d <= x:
        if x % d == 0:
            factors.append(d)
            while x % d == 0:
                x //= d
        d += 1
    if x > 1:
        factors.append(x)
    g = 2
    while True:
        ok = True
        for q in factors:
            if pow(g, phi // q, p) == 1:
                ok = False
                break
        if ok:
            return g
        g += 1


def solve_case(n, p):
    if p == 2:
        return "-1"
    g = gcd(n, p - 1)
    rt = primitive_root(p)
    step = pow(rt, g, p)
    n_div_g = n // g
    phi_div_g = (p - 1) // g
    inv_n_div_g = mod_inverse(n_div_g, phi_div_g)
    seen = {}
    cur = 1
    cnt = 0
    while True:
        cur = (cur * step) % p
        cnt += 1
        seen[cur] = cnt
        if cur == 1:
            return "-1"
        other = (1 - cur) % p
        if other in seen:
            kx = cnt
            ky = seen[other]
            ex = (kx * inv_n_div_g) % (p - 1)
            ey = (ky * inv_n_div_g) % (p - 1)
            x = pow(rt, ex, p)
            y = pow(rt, ey, p)
            return f"{x} {y} 1"


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    t = int(data[0])
    idx = 1
    ans = []
    for _ in range(t):
        n = int(data[idx])
        p = int(data[idx + 1])
        idx += 2
        ans.append(solve_case(n, p))
    sys.stdout.write("\n".join(ans))


if __name__ == "__main__":
    main()
