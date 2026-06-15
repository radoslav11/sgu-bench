import sys
sys.setrecursionlimit(10**7)

def extended_gcd(a, b):
    """
    Returns (g, x, y) such that a*x + b*y = g = gcd(a,b).
    """
    if b == 0:
        return (a, 1, 0)
    g, x1, y1 = extended_gcd(b, a % b)
    # After recursion: b*x1 + (a%b)*y1 = g
    # => a*y1 + b*(x1 - (a//b)*y1) = g
    return (g, y1, x1 - (a // b) * y1)

def solve():
    data = sys.stdin.read().split()
    it = iter(data)
    n, p, b = map(int, (next(it), next(it), next(it)))
    a = [int(next(it)) % p for _ in range(n)]
    # Append modulus term to absorb it into the same system
    a.append(p)

    # We'll compute coefficients x[i] so that sum(a[i]*x[i]) = g
    m = n + 1
    x = [0]*m

    # Start from the last two to get their gcd
    g, x[m-2], x[m-1] = extended_gcd(a[m-2], a[m-1])
    # Inductively incorporate earlier a[i]
    for i in range(m-3, -1, -1):
        prev_g = g
        g, xi, mult = extended_gcd(a[i], prev_g)
        x[i] = xi
        # Each previous coefficient x[j>i] must be multiplied by 'mult'
        for j in range(i+1, m):
            x[j] *= mult

    # Now we have sum(a[i]*x[i]) = g. We need g | b.
    if b % g != 0:
        print("NO")
        return

    # Otherwise we can form a solution
    print("YES")
    scale = b // g
    res = []
    # Drop the last coefficient (for p) and scale the rest
    for xi in x[:-1]:
        xi = (xi * scale) % p
        res.append(str(xi))
    # Output X1..XN
    print(" ".join(res))

if __name__ == "__main__":
    solve()
