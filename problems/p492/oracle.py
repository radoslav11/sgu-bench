import sys

sys.setrecursionlimit(10000)


def count_upto(bound):
    s = str(bound)
    L = len(s)
    dp = [[[None] * 2 for _ in range(2)] for __ in range(L)]

    def rec(pos, last1, tight):
        if pos == L:
            return 1
        if dp[pos][last1][tight] is not None:
            return dp[pos][last1][tight]
        limit = int(s[pos]) if tight else 9
        res = 0
        for d in range(0, limit + 1):
            if last1 and d == 3:
                continue
            nxt_last1 = 1 if d == 1 else 0
            nxt_tight = tight and (d == limit)
            res += rec(pos + 1, nxt_last1, nxt_tight)
        dp[pos][last1][tight] = res
        return res

    return rec(0, 0, True)


def find_nth(n):
    lo, hi = 1, 2 * n
    ans = hi
    while lo <= hi:
        mid = (lo + hi) // 2
        if count_upto(mid) - 1 >= n:
            ans = mid
            hi = mid - 1
        else:
            lo = mid + 1
    return ans


def main():
    data = sys.stdin.read().split()
    T = int(data[0])
    idx = 1
    out = []
    for _ in range(T):
        n = int(data[idx])
        idx += 1
        out.append(str(find_nth(n)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
