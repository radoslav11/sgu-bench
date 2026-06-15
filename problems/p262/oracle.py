import sys

def solve() -> None:
    data = sys.stdin.read().split()
    it = iter(data)

    # Read N, M, K
    n = int(next(it))
    m = int(next(it))
    k = int(next(it))

    total = n * m

    # Read K symbols, each consists of N strings of length M.
    # Store each symbol as a flat list of ints (0/1) of length total.
    symbols = []
    for _ in range(k):
        flat = []
        for _ in range(n):
            row = next(it)
            flat.extend(1 if c == '1' else 0 for c in row)
        symbols.append(flat)

    # Build list of all unordered symbol pairs (a<b).
    pair_list = []
    for a in range(k):
        for b in range(a + 1, k):
            pair_list.append((a, b))
    pairs = len(pair_list)                 # <= 15
    full_mask = (1 << pairs) - 1

    # Precompute for each pixel p: bitmask of which symbol pairs differ at p.
    pixel_mask = [0] * total
    for p in range(total):
        pm = 0
        for i, (a, b) in enumerate(pair_list):
            if symbols[a][p] != symbols[b][p]:
                pm |= (1 << i)
        pixel_mask[p] = pm

    INF = total + 1

    # dp[p][mask] = min selected pixels using first p pixels to achieve 'mask'.
    # Use a list of lists; size (total+1) x (full_mask+1).
    dp = [[INF] * (full_mask + 1) for _ in range(total + 1)]
    prev = [[-1] * (full_mask + 1) for _ in range(total + 1)]

    dp[0][0] = 0
    prev[0][0] = 0

    # Standard 0/1 DP over pixels.
    for p in range(total):
        for mask in range(full_mask + 1):
            cur = dp[p][mask]
            if cur >= INF:
                continue

            # Option 1: skip pixel p
            if dp[p + 1][mask] > cur:
                dp[p + 1][mask] = cur
                prev[p + 1][mask] = mask

            # Option 2: take pixel p
            new_mask = mask | pixel_mask[p]
            if dp[p + 1][new_mask] > cur + 1:
                dp[p + 1][new_mask] = cur + 1
                prev[p + 1][new_mask] = mask

    # Reconstruct which pixels were chosen by backtracking.
    chosen = [0] * total
    mask = full_mask
    for p in range(total, 0, -1):
        pm = prev[p][mask]
        # If mask changed, pixel p-1 was taken; otherwise it was skipped.
        if pm != mask:
            chosen[p - 1] = 1
        mask = pm

    # Output
    out = []
    out.append(str(dp[total][full_mask]))
    for i in range(n):
        row = chosen[i * m:(i + 1) * m]
        out.append("".join('1' if x else '0' for x in row))

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
