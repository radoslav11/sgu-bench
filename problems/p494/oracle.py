import sys
sys.setrecursionlimit(1_000_000)

def solve_case(n, w, r, c, lens_input):
    # lens[0..n-1] are real words; lens[n] is sentinel 0
    lens = lens_input[:] + [0]

    # Precompute len_next[v][space] for v in [0..n], space in [0..w]
    # Only spaces up to (w - c) are needed for illustrated segments, but w is small enough.
    len_next = [[n] * (w + 1) for _ in range(n + 1)]
    max_seg = w - c

    for v in range(n + 1):
        nxt = min(n, v + 1)
        lv = lens[v]
        if lv <= w:
            len_next[v][lv] = nxt

        gap = 0
        # Fill for k = lv+1 .. max_seg
        for k in range(lv + 1, max_seg + 1):
            gap += 1
            if nxt < n and gap >= 1 + lens[nxt]:
                nxt += 1
                gap = 0
            len_next[v][k] = nxt

    def fit(v, space):
        """Return first word index not fitting in a contiguous segment of length space."""
        if v == n or space < lens[v]:
            return v
        return len_next[v][space]

    def advance(v, prev):
        """
        Normal-layout greedy placement transition.
        prev = (row, col_start_of_prev_word)
        returns (row, col_start_of_word_v)
        Words are 1-indexed in this transition, matching the C++ code style.
        """
        row, col_prev = prev
        # Move to after previous word and one mandatory space (unless v == n sentinel-ish)
        col = col_prev + lens[v - 1] + (1 if v < n else 0)
        if col + lens[v] > w:
            return (row + 1, 0)
        return (row, col)

    # Compute row_start for the base layout (no illustration)
    row_start = [0] * (n + 1)
    cur = (0, 0)
    num_lines = 0
    for j in range(1, n + 1):
        cur = advance(j, cur)
        if cur[1] == 0:
            row_start[cur[0]] = j
        if j == n - 1:
            num_lines = cur[0]

    # best[i] = best known (row, col) before placing word i
    INF = n + r + 10
    best = [(INF, 0)] * (n + 1)

    # Per-column path compression structure
    class Comp:
        __slots__ = ("result", "steps")
        def __init__(self):
            self.result = 0
            self.steps = 0

    comp = [Comp() for _ in range(n + 1)]

    def make_compress(gap_left, gap_right):
        # Reset memo for this column
        for i in range(n + 1):
            comp[i].result = 0
            comp[i].steps = 0

        def jump(v, steps):
            """Compute next^steps(v) with memoized compression."""
            if v == n or steps == 0:
                return v

            if comp[v].steps != 0:
                # Extend from stored result if needed
                if comp[v].result != v:
                    comp[v].result = jump(comp[v].result, steps - comp[v].steps)
            else:
                nxt = fit(fit(v, gap_left), gap_right)
                comp[v].result = jump(nxt, steps - 1) if nxt != v else nxt

            comp[v].steps = steps
            return comp[v].result

        return jump

    # Try all illustration columns
    for col in range(0, w - c + 1):
        jump = make_compress(col, w - col - c)
        for j in range(0, num_lines + 1):
            end = jump(row_start[j], r)
            candidate = (j + r - (1 if end == n else 0), 0)
            if candidate < best[end]:
                best[end] = candidate

    # Propagate remaining words with normal greedy layout
    for j in range(0, n):
        cand = advance(j + 1, best[j])
        if cand < best[j + 1]:
            best[j + 1] = cand

    return best[n][0] + 1  # row index + 1 => number of used rows


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    t = data[0]
    idx = 1
    out = []
    for _ in range(t):
        n, w, r, c = data[idx:idx+4]
        idx += 4
        lens = data[idx:idx+n]
        idx += n
        out.append(str(solve_case(n, w, r, c, lens)))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
