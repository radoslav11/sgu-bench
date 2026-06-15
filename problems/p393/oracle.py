import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    edges = [next(it) for _ in range(m)]

    # Build adjacency bitmasks:
    out_edges = [0] * n
    in_edges = [0] * n
    for e in edges:
        u = ord(e[0]) - 97
        v = ord(e[1]) - 97
        out_edges[u] |= 1 << v
        in_edges[v]  |= 1 << u

    masks = 1 << n

    # any_out[mask]  = union of out-neighbors of vertices in mask
    # dang_out[mask] = set of vertices that receive >=2 edges from vertices in mask
    # int_cnt[mask]  = number of directed edges fully inside mask (incl. loops)
    any_out = [0] * masks
    dang_out = [0] * masks
    int_cnt = [0] * masks

    # Precompute using subset DP:
    for mask in range(1, masks):
        # Extract least significant set bit and its index.
        lsb = mask & -mask
        u = (lsb.bit_length() - 1)      # index of lsb
        prev = mask ^ lsb

        any_out[mask] = any_out[prev] | out_edges[u]
        dang_out[mask] = dang_out[prev] | (any_out[prev] & out_edges[u])

        # Count edges fully inside mask added by introducing u:
        # u -> v for v in prev
        add1 = (out_edges[u] & prev).bit_count()
        # v -> u for v in prev  (i.e., incoming to u from prev)
        add2 = (in_edges[u] & prev).bit_count()
        # self-loop u -> u
        loop = (out_edges[u] >> u) & 1

        int_cnt[mask] = int_cnt[prev] + add1 + add2 + loop

    def compatible(g: int, h: int) -> bool:
        # Check g -> h has at most 1 edge:
        if dang_out[g] & h:
            return False
        if (any_out[g] & h).bit_count() > 1:
            return False

        # Check h -> g has at most 1 edge:
        if dang_out[h] & g:
            return False
        if (any_out[h] & g).bit_count() > 1:
            return False

        return True

    best_k = n
    best_groups = [(1 << i) for i in range(n)]  # default: singletons

    groups = [0] * n  # current solution under construction

    sys.setrecursionlimit(1_000_000)

    def feasible(k: int) -> bool:
        """
        Try to find a partition of all letters into <= k groups.
        If found, store it into best_groups/best_k and return True.
        """
        nonlocal best_k, best_groups

        found = False

        def dfs(remaining: int, used: int) -> None:
            nonlocal found, best_k, best_groups
            if found:
                return
            if remaining == 0:
                # Success: we built a complete partition.
                found = True
                best_k = used
                best_groups[:used] = groups[:used]
                return
            if used >= k:
                return

            # Force next group to include the lowest remaining letter.
            lo = remaining & -remaining
            rest = remaining ^ lo

            sub = rest
            while True:
                group = sub | lo

                # Internal constraint: at most one edge inside the group.
                if int_cnt[group] <= 1:
                    ok = True
                    for i in range(used):
                        if not compatible(group, groups[i]):
                            ok = False
                            break
                    if ok:
                        groups[used] = group
                        dfs(remaining ^ group, used + 1)
                        if found:
                            return

                if sub == 0:
                    break
                sub = (sub - 1) & rest  # next subset

        dfs((1 << n) - 1, 0)
        return found

    # Binary search minimum k
    left, right = 1, n
    while left <= right:
        mid = (left + right) // 2
        if feasible(mid):
            right = mid - 1
        else:
            left = mid + 1

    # Print result
    out_lines = [str(best_k)]
    for i in range(best_k):
        mask = best_groups[i]
        letters = []
        for j in range(n):
            if (mask >> j) & 1:
                letters.append(chr(97 + j))
        out_lines.append("".join(letters))
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
