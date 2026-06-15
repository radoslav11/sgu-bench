import sys
sys.setrecursionlimit(10**7)

def read_input():
    data = sys.stdin.read().split()
    n = int(data[0])
    a = list(map(int, data[1:]))
    return n, a

def prepare(a, n):
    # pref_cnt_1[k][i] = among the first i piles in 'order' for bit k,
    # how many have bit k = 1
    pref_cnt_1 = [[0]*(n+1) for _ in range(61)]
    order = list(range(n))
    for bit in range(61):
        zero_bucket = []
        one_bucket = []
        cnt1 = 0
        for i, idx in enumerate(order):
            b = (a[idx] >> bit) & 1
            if b:
                one_bucket.append(idx)
            else:
                zero_bucket.append(idx)
            cnt1 += b
            pref_cnt_1[bit][i+1] = cnt1
        # new order: zeros then ones
        order = zero_bucket + one_bucket
    return pref_cnt_1

def solve():
    n, a = read_input()
    pref_cnt_1 = prepare(a, n)

    # dp[bit][carry_cnt]: number of ways from bit..60 with carry_cnt incoming borrows
    from functools import lru_cache
    @lru_cache(None)
    def dp(bit, carry):
        # If past final bit, valid only if no carry remains
        if bit == 61:
            return 1 if carry == 0 else 0
        res = 0
        # Precompute counts for this bit
        total_ones = pref_cnt_1[bit][n]
        ones1 = pref_cnt_1[bit][carry]          # among piles with incoming borrow=1
        zeros1 = carry - ones1
        ones0 = total_ones - ones1             # among piles with incoming borrow=0
        zeros0 = (n - carry) - ones0

        # Try x_bit = 0 or 1
        for xbit in (0,1):
            xor_par = 0
            new_c = 0
            # group b=0,v=0
            cnt = zeros0
            val = (0 - xbit) & 1
            xor_par ^= val * (cnt & 1)
            if 0 - xbit < 0: new_c += cnt
            # group b=0,v=1
            cnt = ones0
            val = (1 - xbit) & 1
            xor_par ^= val * (cnt & 1)
            if 1 - xbit < 0: new_c += cnt
            # group b=1,v=0
            cnt = zeros1
            val = (0 - xbit - 1) & 1
            xor_par ^= val * (cnt & 1)
            if 0 - xbit - 1 < 0: new_c += cnt
            # group b=1,v=1
            cnt = ones1
            val = (1 - xbit - 1) & 1
            xor_par ^= val * (cnt & 1)
            if 1 - xbit - 1 < 0: new_c += cnt

            if xor_par == 0:
                res += dp(bit+1, new_c)
        return res

    # Total ways for x >= 0
    total = dp(0, 0)
    # Subtract invalid x = min(a)
    mn = min(a)
    xor_mn = 0
    for v in a:
        xor_mn ^= (v - mn)
    if xor_mn == 0:
        total -= 1

    print(total)

if __name__ == "__main__":
    solve()
