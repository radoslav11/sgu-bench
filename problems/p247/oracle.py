import sys
import math

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    t = int(data[0])
    out = []
    idx = 1

    for _ in range(t):
        p = int(data[idx]); idx += 1

        # Compute C(2p, p) as an exact integer.
        # Python ints are arbitrary precision, so no custom bigint needed.
        #
        # Multiplicative formula:
        # C(2p, p) = Π_{i=1..p} (p+i)/i = Π_{i=0..p-1} (2p-i)/(i+1)
        c = 1
        for i in range(p):
            c *= (2 * p - i)
            c //= (i + 1)   # exact division at every step

        # Apply derived counting formula:
        # answer = 2 + (C(2p,p) - 2) / p
        ans = 2 + (c - 2) // p
        out.append(str(ans))

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
