import sys
sys.setrecursionlimit(10000)
from functools import lru_cache

def count_almost_lucky_up_to(n: int) -> int:
    """
    Returns the number of almost-lucky numbers <= n.
    """

    s = str(n)
    nd = len(s)
    res = 0

    # We only consider even digit lengths 2,4,6,8,10
    for length in range(2, nd+1, 2):
        # Build bound string for this length
        if length < nd:
            bound = '9' * length
        else:
            bound = s

        L = len(bound)
        N = L // 2

        @lru_cache(None)
        def dp(pos: int, balance: int, max_inc: int, max_dec: int, tight: bool) -> int:
            """
            pos      : current digit position [0..L]
            balance  : sum(first half) - sum(second half)
            max_inc  : best possible increase from any digit so far
            max_dec  : best possible decrease from any digit so far
            tight    : whether prefix == bound[:pos]
            """
            # Base case: completed all digits
            if pos == L:
                # If already lucky, not almost-lucky
                if balance == 0:
                    return 0
                # If balance < 0, we need to increase some digit by at least -balance
                if balance < 0 and max_inc >= -balance:
                    return 1
                # If balance > 0, we need to decrease some digit by at least balance
                if balance > 0 and max_dec >= balance:
                    return 1
                return 0

            total = 0
            up = int(bound[pos]) if tight else 9
            # No leading zero
            start = 1 if pos == 0 else 0

            for d in range(start, up+1):
                nb = balance
                ni = max_inc
                ndc = max_dec
                # First half: contributes +d
                if pos < N:
                    nb += d
                    # Decrease: can lower d to 0 (or 1 if leading)
                    dec_possible = d - 1 if pos == 0 else d
                    ndc = max(ndc, dec_possible)
                    # Increase: to 9
                    ni = max(ni, 9 - d)
                else:
                    # Second half: contributes -d
                    nb -= d
                    # Increase: raising this digit from d→9 gives +d
                    ni = max(ni, d)
                    # Decrease: lowering from d→0 gives +(9-d)
                    ndc = max(ndc, 9 - d)

                nt = tight and (d == up)
                total += dp(pos+1, nb, ni, ndc, nt)

            return total

        res += dp(0, 0, 0, 0, True)

    return res

def main():
    A, B = map(int, sys.stdin.read().split())
    ans = count_almost_lucky_up_to(B)
    if A > 0:
        ans -= count_almost_lucky_up_to(A-1)
    print(ans)

if __name__ == "__main__":
    main()
