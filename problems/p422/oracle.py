import sys


def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    t = float(next(it))
    a = [float(next(it)) for _ in range(n)]

    # dp[k] = minimal expected remaining time given first k characters are correct
    dp = [0.0] * (n + 1)   # dp[n] = 0 base case

    # Compute backwards
    for k in range(n - 1, -1, -1):
        best = 1e100

        q = 1.0       # probability all typed-so-far in current block are correct
        c1 = 0.0      # accumulator (see C++ code / editorial)
        c2 = 0.0      # accumulator
        b_val = 0.0   # accumulator of dp contributions

        ak = a[k]
        # try p = 1..(n-k) characters typed before looking
        for p in range(1, n - k + 1):
            # update q = prod_{i=0..p-1} (1 - a[k+i])
            q *= (1.0 - a[k + p - 1])

            # same rearranged expectation numerator as in the C++ solution
            g = (
                p * (1.0 + ak + c1)
                - c2
                + t
                + b_val
                + q * dp[k + p]
            )

            # rearranged division to remove the dp[k] term on RHS
            cand = g / (1.0 - ak)
            if cand < best:
                best = cand

            # update accumulators for the next p (if there is a next char)
            if p < n - k:
                term = q * a[k + p]     # prob first p correct and next wrong
                c1 += term
                c2 += term * p
                b_val += term * dp[k + p]

        dp[k] = best

    # print with enough precision
    sys.stdout.write("{:.15f}\n".format(dp[0]))


if __name__ == "__main__":
    solve()
