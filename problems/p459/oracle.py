import sys

def rec(n: int, k: int) -> list[int]:
    """
    Return a list of bitmasks of length n enumerating all subsets
    with popcount <= k exactly once, in an order where consecutive
    masks differ by either:
      - 1 bit, or
      - 2 adjacent bits (allowed swap of similar dancers).
    """
    if k == 0:
        return [0]

    # First half: top bit is 0, so we just enumerate on n-1 bits with <= k ones.
    a = rec(n - 1, min(k, n - 1))

    # Second half: top bit is 1, so remaining n-1 bits have <= k-1 ones.
    b = rec(n - 1, k - 1)

    top = 1 << (n - 1)

    # Append reversed b with the top bit set (Gray-code style reflection).
    # Reversal is crucial to make the boundary transition valid.
    for x in reversed(b):
        a.append(x | top)

    return a

def solve(n: int, k: int) -> str:
    gray = rec(n, k)

    # End with empty stage
    gray.append(0)

    out = []

    # For each transition prev -> cur, output the appropriate operation
    for j in range(1, len(gray)):
        prev = gray[j - 1]
        cur = gray[j]

        # Collect differing bit positions
        pos = []
        diff = prev ^ cur
        for i in range(n):
            if (diff >> i) & 1:
                pos.append(i)

        if len(pos) == 1:
            p = pos[0]
            if (cur >> p) & 1:
                out.append(f"+{p+1}")   # dancer appears
            else:
                out.append(f"-{p+1}")   # dancer leaves
        else:
            # Two-bit adjacent toggle => swap with neighbor.
            # The construction guarantees len(pos)==2 and adjacent.
            p = pos[0]  # lower index (since we scan increasing i)
            if (cur >> p) & 1:
                # Now lower bit is 1: interpret as "--(p+2)"
                out.append(f"--{p+2}")
            else:
                # Now lower bit is 0: interpret as "++(p+1)"
                out.append(f"++{p+1}")

    return "".join(out)

def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0]); k = int(data[1])
    sys.stdout.write(solve(n, k))

if __name__ == "__main__":
    main()
