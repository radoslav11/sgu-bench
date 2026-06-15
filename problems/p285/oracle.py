import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    n = int(next(it))

    # Read 12 sector probabilities for sectors 1..12
    sector = [float(next(it)) for _ in range(12)]

    # Read N Internet probabilities and compute their average
    internet_probs = [float(next(it)) for _ in range(n)]
    avg_internet = sum(internet_probs) / n

    # dp[mask][c][v]:
    # mask in [0, 2^13), c,v in [0..5]
    # Using a flat list is faster in Python than deep nested lists of lists,
    # but 3D list is still OK at this size; we’ll use a flattened array for speed.
    #
    # index = (mask * 36) + (c * 6) + v
    M = 1 << 13
    dp = [0.0] * (M * 36)
    dp[0] = 1.0  # mask=0, c=0, v=0

    ans = [0.0] * 12  # required final score probabilities

    def idx(mask: int, c: int, v: int) -> int:
        return mask * 36 + c * 6 + v

    for mask in range(M):
        # Precompute availability of each sector 1..13 for this mask
        # bit 0..11 => sectors 1..12, bit 12 => sector 13
        avail = [False] * 14
        for s in range(1, 13):
            avail[s] = (mask & (1 << (s - 1))) == 0
        avail[13] = (mask & (1 << 12)) == 0

        # Find wrap_target: first available sector in 1..13
        wrap_target = -1
        for s in range(1, 14):
            if avail[s]:
                wrap_target = s
                break

        # If none available, nothing to do
        if wrap_target == -1:
            continue

        # Compute freq[s]: number of initial landings mapping to chosen sector s
        freq = [0] * 14
        target = wrap_target
        for s in range(13, 0, -1):
            if avail[s]:
                target = s
            freq[target] += 1

        for c in range(6):
            base_c = mask * 36 + c * 6
            for v in range(6):
                cur = dp[base_c + v]
                if cur < 1e-18:
                    continue

                # Try each possible chosen sector s
                for s in range(1, 14):
                    f = freq[s]
                    if f == 0:
                        continue

                    # Probability that this round chooses sector s
                    land = cur * (f / 13.0)

                    # Probability club answers correctly
                    p = sector[s - 1] if s <= 12 else avg_internet

                    # Update mask by removing this sector's envelope
                    if s <= 12:
                        new_mask = mask | (1 << (s - 1))
                    else:
                        new_mask = mask | (1 << 12)

                    # Club correct answer
                    if c + 1 == 6:
                        # final score 6:v
                        ans[v] += land * p
                    else:
                        dp[idx(new_mask, c + 1, v)] += land * p

                    # Club wrong answer (viewers gain point)
                    if v + 1 == 6:
                        # final score c:6; output order is 5:6..0:6
                        ans[6 + (5 - c)] += land * (1.0 - p)
                    else:
                        dp[idx(new_mask, c, v + 1)] += land * (1.0 - p)

    out = "\n".join(f"{x:.6f}" for x in ans)
    sys.stdout.write(out + "\n")

if __name__ == "__main__":
    solve()
