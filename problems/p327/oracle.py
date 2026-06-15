import sys

def solve_clean():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n_in = int(next(it))
    all_words = [next(it) for _ in range(n_in)]

    all_words = sorted(set(all_words), key=len)

    kept = []
    for i, w in enumerate(all_words):
        rw = w[::-1]
        ok = True
        for j, u in enumerate(all_words):
            if i == j or len(u) <= len(w):
                continue
            ru = u[::-1]
            if (w in u) or (rw in u) or (w in ru) or (rw in ru):
                ok = False
                break
        if ok:
            kept.append(w)

    words = [(w, w[::-1]) for w in kept]
    n = len(words)
    m = 2 * n

    overlap = [[0] * m for _ in range(m)]
    for a in range(m):
        wa = words[a // 2][a % 2]
        for b in range(m):
            if a // 2 == b // 2:
                continue
            wb = words[b // 2][b % 2]
            lim = min(len(wa), len(wb))
            best = 0
            for k in range(1, lim + 1):
                if wa[-k:] == wb[:k]:
                    best = k
            overlap[a][b] = best

    def longest_pal_prefix(s: str) -> int:
        best = 1
        for L in range(1, len(s) + 1):
            pref = s[:L]
            if pref == pref[::-1]:
                best = L
        return best

    FULL = (1 << n) - 1
    INF = 10**9
    dp = [[INF] * m for _ in range(1 << n)]
    prev = [[-1] * m for _ in range(1 << n)]

    start_half = [""] * m
    start_odd = [False] * m

    for i in range(n):
        for rev in (0, 1):
            c = 2 * i + rev
            w = words[i][rev]
            best_pal = longest_pal_prefix(w)
            half = w[best_pal // 2:]
            odd = (best_pal % 2 == 1)
            dp[1 << i][c] = 2 * len(half) - (1 if odd else 0)
            start_half[c] = half
            start_odd[c] = odd

    for mask in range(1, FULL + 1):
        for c in range(m):
            cur = dp[mask][c]
            if cur >= INF:
                continue
            for j in range(n):
                if mask & (1 << j):
                    continue
                for rev in (0, 1):
                    nc = 2 * j + rev
                    ov = overlap[c][nc]
                    cost = 2 * (len(words[j][rev]) - ov)
                    nmask = mask | (1 << j)
                    nl = cur + cost
                    if nl < dp[nmask][nc]:
                        dp[nmask][nc] = nl
                        prev[nmask][nc] = c

    best_c = min(range(m), key=lambda c: dp[FULL][c])

    # Backtrack
    path = []
    mask = FULL
    c = best_c
    while True:
        path.append(c)
        pc = prev[mask][c]
        if pc == -1:
            break
        mask ^= (1 << (c // 2))  # remove current word from mask
        c = pc
    path.reverse()

    # Build right half
    suf = start_half[path[0]]
    for i in range(1, len(path)):
        prev_c = path[i - 1]
        c = path[i]
        ov = overlap[prev_c][c]
        suf += words[c // 2][c % 2][ov:]

    rev_suf = suf[::-1]
    if start_odd[path[0]]:
        ans = rev_suf[:-1] + suf
    else:
        ans = rev_suf + suf

    sys.stdout.write(ans + "\n")

if __name__ == "__main__":
    solve_clean()
