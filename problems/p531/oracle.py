import sys
import bisect

def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    n = int(next(it))
    d = int(next(it))
    # Read banks as (x, w), in sorted order by x
    a = [(int(next(it)), int(next(it))) for _ in range(n)]

    # Build prefix max: pref[i] = (max_w so far, index_of_bank)
    pref = [None] * n
    max_w, max_idx = a[0][1], 0
    pref[0] = (max_w, 0)
    for i in range(1, n):
        w = a[i][1]
        if w > max_w:
            max_w, max_idx = w, i
        pref[i] = (max_w, max_idx)

    # Build suffix max: suff[i] = (max_w from i..n-1, index_of_bank)
    suff = [None] * n
    max_w, max_idx = a[n-1][1], n-1
    suff[n-1] = (max_w, max_idx)
    for i in range(n-2, -1, -1):
        w = a[i][1]
        if w >= max_w:  # >= so later index preferred if equal
            max_w, max_idx = w, i
        suff[i] = (max_w, max_idx)

    # Extract x coordinates for binary search
    xs = [x for x, _ in a]

    best_sum = 0
    ans = (-1, -1)

    # For each left bank i
    for i in range(n):
        xi, wi = a[i]
        target = xi + d
        # Find the first j with xj >= xi + d
        j = bisect.bisect_left(xs, target)
        if j == n:
            continue  # no valid right bank
        # Best left in [0..i], best right in [j..n-1]
        wl, idx_l = pref[i]
        wr, idx_r = suff[j]
        total = wl + wr
        if total >= best_sum:
            best_sum = total
            ans = (idx_l + 1, idx_r + 1)  # convert to 1-based

    print(ans[0], ans[1])

if __name__ == "__main__":
    main()
