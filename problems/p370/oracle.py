import sys
import threading

def main():
    data = sys.stdin.read().split()
    n, m = map(int, data)
    # A = n-1, B = m-1
    A, B = n - 1, m - 1

    # Count all pairs dx>=1, dy>=1 unconstrained by gcd
    total_pairs = A * B

    # We will build dp[d] = number of pairs (dx,dy) with gcd(dx,dy)==d
    # for d from 1..lim, but we only need d>=2 to subtract non-coprime.
    lim = min(A, B)
    dp = [0] * (lim + 1)

    # Inclusion–exclusion from largest d down to 2
    for d in range(lim, 1, -1):
        # pairs where dx and dy share factor d
        cnt = (A // d) * (B // d)
        # subtract those already attributed to multiples of d
        multiple = 2 * d
        while multiple <= lim:
            cnt -= dp[multiple]
            multiple += d
        dp[d] = cnt
        total_pairs -= cnt  # remove all non-coprime pairs

    # total_pairs now equals count of dx>=1,dy>=1,gcd=1
    # plus horizontal shot if n>1 (dx>0,dy=0) and vertical if m>1
    shots = total_pairs
    if n > 1:
        shots += 1
    if m > 1:
        shots += 1

    print(shots)

if __name__ == "__main__":
    threading.Thread(target=main).start()
