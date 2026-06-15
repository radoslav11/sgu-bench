import sys

MOD = 10**9 + 7

def mod_pow(a: int, e: int, mod: int = MOD) -> int:
    """Fast exponentiation: return a^e mod mod."""
    r = 1
    a %= mod
    while e > 0:
        if e & 1:
            r = (r * a) % mod
        a = (a * a) % mod
        e >>= 1
    return r

def solve() -> None:
    # Read three capital bitstrings (strip newline)
    cap = [sys.stdin.readline().strip(),
           sys.stdin.readline().strip(),
           sys.stdin.readline().strip()]

    n = len(cap[0])

    # x0: all equal bits across the 3 capitals
    # x1: capital1 is the odd one out (differs from both others)
    # x2: capital2 odd, x3: capital3 odd
    x0 = x1 = x2 = x3 = 0

    # Classify each coordinate
    for i in range(n):
        a = ord(cap[0][i]) - 48
        b = ord(cap[1][i]) - 48
        c = ord(cap[2][i]) - 48

        if a == b == c:
            x0 += 1
        elif a != b and a != c:
            x1 += 1
        elif b != a and b != c:
            x2 += 1
        else:
            x3 += 1

    # Precompute factorials up to max(x1,x2,x3)
    m = max(x1, x2, x3)

    fact = [1] * (m + 1)
    for i in range(1, m + 1):
        fact[i] = (fact[i - 1] * i) % MOD

    inv_fact = [1] * (m + 1)
    # inv_fact[m] = (fact[m])^{-1} mod MOD
    inv_fact[m] = mod_pow(fact[m], MOD - 2)
    # Fill downward
    for i in range(m, 0, -1):
        inv_fact[i - 1] = (inv_fact[i] * i) % MOD

    def nCk(n: int, k: int) -> int:
        """Compute binomial coefficient C(n,k) mod MOD."""
        if k < 0 or k > n:
            return 0
        return (fact[n] * inv_fact[k] % MOD) * inv_fact[n - k] % MOD

    ans = 0

    # Iterate k2; derive k1, k3 from neutrality equations:
    # 2*(k1 - k2) = x1 - x2
    # 2*(k2 - k3) = x2 - x3
    for k2v in range(x2 + 1):
        two_k1 = 2 * k2v + (x1 - x2)
        two_k3 = 2 * k2v + (x3 - x2)

        # Must be nonnegative and even to form integer k1,k3
        if two_k1 < 0 or two_k3 < 0:
            continue
        if (two_k1 & 1) or (two_k3 & 1):
            continue

        k1v = two_k1 // 2
        k3v = two_k3 // 2

        # Must be within bounds
        if k1v > x1 or k3v > x3:
            continue

        ways = nCk(x1, k1v)
        ways = (ways * nCk(x2, k2v)) % MOD
        ways = (ways * nCk(x3, k3v)) % MOD
        ans = (ans + ways) % MOD

    # Multiply by 2^x0 for free coordinates
    ans = (ans * mod_pow(2, x0)) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
