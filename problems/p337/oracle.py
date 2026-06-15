import sys

def main():
    data = sys.stdin.read().strip().splitlines()
    if not data:
        print()
        return
    k = int(data[0].strip())
    s = data[1].strip()
    m = len(s)

    # Double the string to model cyclic substrings as contiguous segments
    s2 = s + s
    n = len(s2)  # n = 2 * m

    best = None  # best answer string or None

    # We must not exceed the original (cyclic) length m.
    # Only even lengths are valid: L = 2, 4, ..., up to m (if m even) or m-1 (if m odd).
    max_even_L = m if (m % 2 == 0) else (m - 1)

    # Try lengths from largest even downwards; stop when we find at least one candidate.
    for L in range(max_even_L, 1 - 1, -2):
        d = L // 2  # half-length; shift between halves

        # For this shift d, define eq[i] = 1 if s2[i] != s2[i + d], else 0.
        # Valid i are 0..n - d - 1.
        eq_len = n - d
        eq = [0] * eq_len
        for i in range(eq_len):
            eq[i] = 1 if s2[i] != s2[i + d] else 0

        # Build prefix sums to query sum(eq[x .. x + d - 1]) quickly.
        pref = [0] * (eq_len + 1)
        for i in range(eq_len):
            pref[i + 1] = pref[i] + eq[i]

        # For cyclic substrings of length L, valid starts x satisfy x + L <= n
        # (since we already doubled).
        # Also we only need x up to n - 2d (because L = 2d).
        start_max = n - L
        local_best = None  # best substring for this length L

        for x in range(start_max + 1):
            # mismatches between halves at this start x:
            mism = pref[x + d] - pref[x]
            if mism <= k:
                cand = s2[x : x + L]
                if (local_best is None) or (cand < local_best):
                    local_best = cand

        if local_best is not None:
            best = local_best
            break  # we found the maximum possible length; no need to try smaller L

    # If nothing found, print blank line as required.
    print(best if best is not None else "")

if __name__ == "__main__":
    main()
