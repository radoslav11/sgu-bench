import sys
sys.setrecursionlimit(10000)
from array import array

POS = 19
SUM_MAX = 9 * POS


def digits_lsd_first(x):
    s = str(x)[::-1]
    d = [0] * POS
    for i, ch in enumerate(s[:POS]):
        d[i] = ord(ch) - 48
    return d


def solve():
    data = sys.stdin.read().strip().split()
    L, R, K = int(data[0]), int(data[1]), int(data[2])

    digL = digits_lsd_first(L)
    digR = digits_lsd_first(R)

    memo_block = {}

    def build_block(pos, sum_prefix):
        key = (pos, sum_prefix)
        if key in memo_block:
            return memo_block[key]

        if pos == -1:
            ans_arr = array('Q', [0] * K)
            out_arr = array('H', [0] * K)
            for c in range(K):
                total = c + sum_prefix
                if total >= K:
                    ans_arr[c] = 1
                    out_arr[c] = 0
                else:
                    ans_arr[c] = 0
                    out_arr[c] = total
            memo_block[key] = (ans_arr, out_arr)
            return ans_arr, out_arr

        ans_total = array('Q', [0] * K)
        carry_cur = array('H', range(K))

        for d in range(10):
            ans_d, out_d = build_block(pos - 1, sum_prefix + d)
            for c0 in range(K):
                cc = carry_cur[c0]
                ans_total[c0] += ans_d[cc]
                carry_cur[c0] = out_d[cc]

        memo_block[key] = (ans_total, carry_cur)
        return ans_total, carry_cur

    def dfs(pos, carry, tight_low, tight_high, sum_prefix):
        if not tight_low and not tight_high:
            ans_arr, out_arr = build_block(pos, sum_prefix)
            return int(ans_arr[carry]), int(out_arr[carry])

        if pos == -1:
            total = carry + sum_prefix
            if total >= K:
                return 1, 0
            else:
                return 0, total

        res_ans = 0
        lo = digL[pos] if tight_low else 0
        hi = digR[pos] if tight_high else 9

        for d in range(lo, hi + 1):
            nL = tight_low and (d == lo)
            nH = tight_high and (d == hi)
            add_ans, carry = dfs(pos - 1, carry, nL, nH, sum_prefix + d)
            res_ans += add_ans

        return res_ans, carry

    ans, _ = dfs(POS - 1, 0, True, True, 0)
    print(ans)


if __name__ == "__main__":
    solve()
