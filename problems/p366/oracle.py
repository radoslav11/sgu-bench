import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    k = int(next(it))

    ab = [(int(next(it)), int(next(it))) for _ in range(n)]

    MAXV = 50
    OFFSET = MAXV                 # shift diffs by +50 to become non-negative
    MAX_SIZE = 2 * OFFSET + 1     # 101 possible shifted diffs [0..100]
    max_diff = MAX_SIZE
    max_sum = k * MAX_SIZE        # max possible sum of shifted diffs over K picks

    # Group opponents by shifted diff; each entry is (a+b, index)
    groups = [[] for _ in range(max_diff)]
    for i, (a, b) in enumerate(ab, start=1):     # 1-based indices
        diff = a - b + OFFSET
        s = a + b
        groups[diff].append((s, i))

    # Keep only top K by (a+b) in each diff group
    for d in range(max_diff):
        groups[d].sort(reverse=True)   # sorts by first then second; first is sum
        if len(groups[d]) > k:
            groups[d] = groups[d][:k]

    # DP:
    # dp[cnt][sd] = best total (A+B) achievable by selecting (cnt+1) items
    #               with shifted-diff sum = sd
    # Also store chosen indices for reconstruction.
    #
    # We'll store:
    # possible[cnt][sd] as bool
    # bestsum[cnt][sd] as int
    # choice[cnt][sd] as list of indices (kept only for best state)
    possible = [[False] * max_sum for _ in range(k)]
    bestsum  = [[0] * max_sum for _ in range(k)]
    choice   = [[None] * max_sum for _ in range(k)]  # None or list[int]

    # Iterate candidates
    for diff in range(max_diff):
        for s, idx in groups[diff]:

            # transitions for adding item to existing selections
            for cnt in range(k - 2, -1, -1):  # from k-2 down to 0
                for sd in range(0, max_sum - diff):
                    if not possible[cnt][sd]:
                        continue
                    nsd = sd + diff
                    nsum = bestsum[cnt][sd] + s
                    if (not possible[cnt + 1][nsd]) or (bestsum[cnt + 1][nsd] < nsum):
                        possible[cnt + 1][nsd] = True
                        bestsum[cnt + 1][nsd] = nsum
                        # copy indices from previous and add current index
                        choice[cnt + 1][nsd] = choice[cnt][sd] + [idx]

            # base: selecting exactly 1 item
            if diff < max_sum:
                if (not possible[0][diff]) or (bestsum[0][diff] < s):
                    possible[0][diff] = True
                    bestsum[0][diff] = s
                    choice[0][diff] = [idx]

    # Select best among K picks
    target_cnt = k - 1
    best_sd = -1
    best_s = -1
    best_balance = 10**18

    for sd in range(max_sum):
        if not possible[target_cnt][sd]:
            continue
        balance = abs(sd - OFFSET * k)  # equals |A-B|
        if balance < best_balance or (balance == best_balance and bestsum[target_cnt][sd] > best_s):
            best_balance = balance
            best_sd = sd
            best_s = bestsum[target_cnt][sd]

    # Recover A and B:
    # D = A-B = best_sd - OFFSET*K
    D = best_sd - OFFSET * k
    Ssum = best_s  # A+B

    A = (Ssum + D) // 2
    B = (Ssum - D) // 2

    # Output
    res = sorted(choice[target_cnt][best_sd])
    sys.stdout.write(f"{A} {B}\n")
    sys.stdout.write(" ".join(map(str, res)) + "\n")

if __name__ == "__main__":
    solve()
