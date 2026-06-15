import sys
def solve():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    L = int(next(it))
    A = [int(next(it)) for _ in range(n)]
    B = [int(next(it)) for _ in range(n)]

    # Build 2n points: (position, index, type)
    # type = +1 for house, -1 for office.
    pts = []
    for i, x in enumerate(A):
        pts.append((x, i, +1))
    for i, x in enumerate(B):
        pts.append((x, i, -1))

    # Sort by position, then type, then index
    pts.sort(key=lambda p: (p[0], p[2], p[1]))
    # Close the circle by repeating the first point at +L
    pts.append((pts[0][0] + L, pts[0][1], pts[0][2]))

    # Compute gap[i] and balance[i], and the initial cost
    gap = [0]*(2*n)
    bal = [0]*(2*n)
    total_len = 0
    cur_bal = 0
    cost = 0
    for i in range(2*n):
        cur_bal += pts[i][2]
        bal[i] = cur_bal
        gap[i] = pts[i+1][0] - pts[i][0]
        total_len += gap[i]
        cost += gap[i] * cur_bal

    # Prepare events sorted by balance
    order = list(range(2*n))
    order.sort(key=lambda i: bal[i])

    # Start the sweep at k = min balance so that every |bal - k| is
    # simply bal - k and the incremental updates stay valid.
    kmin = min(bal)
    cost = sum(gap[i] * (bal[i] - kmin) for i in range(2*n))
    best_cost = None
    best_idx = 0
    prefix_len = 0
    last_bal = kmin

    # Sweep the cut along the circle across each gap in balance order
    for idx in order:
        d = bal[idx] - last_bal
        # Update cost for crossing this gap
        cost += prefix_len * d - (total_len - prefix_len) * d
        if best_cost is None or cost < best_cost:
            best_cost = cost
            best_idx = idx
        prefix_len += gap[idx]
        last_bal = bal[idx]

    # Output minimal cost
    out = [str(best_cost)]

    # Reconstruct the matching
    ans = [-1]*n
    st = []
    pos = (best_idx + 1) % (2*n)
    for _ in range(2*n):
        x, i, t = pts[pos]
        if not st or st[-1][2] == t:
            # push unmatched endpoint
            st.append((x, i, t))
        else:
            # pop and match
            x2, j, t2 = st.pop()
            if t == +1:
                # current is house, popped was office
                ans[i] = j+1
            else:
                # popped was house, current is office
                ans[j] = i+1
        pos = (pos + 1) % (2*n)

    # Append matching
    out.append(" ".join(map(str, ans)))
    print("\n".join(out))

if __name__ == "__main__":
    solve()
