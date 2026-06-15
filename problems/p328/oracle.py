import sys

def sg_match(length: int) -> int:
    # Both neighbors exist and are the same color.
    # Closed form from the known pattern.
    return 1 if length >= 1 else 0

def sg_diff(length: int) -> int:
    # Both neighbors exist and have different colors.
    return 0

def sg_boundary(length: int) -> int:
    # Segment touches one end of the chain (only one colored neighbor).
    return length

def sg_free(length: int) -> int:
    # Segment touches both ends (entire chain is zeros).
    return length % 2

def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    s = data[1].strip()
    # n is not strictly needed except for safety; s length should be n.

    xor_sum = 0
    i = 0

    # Scan the string and extract maximal zero segments
    while i < n:
        if s[i] == '0':
            start = i
            while i < n and s[i] == '0':
                i += 1
            length = i - start

            # Determine boundary colors; use '\0' sentinel (or None) for "no neighbor"
            left = s[start - 1] if start > 0 else None
            right = s[i] if i < n else None

            # Classify and compute the segment Grundy number
            if left is None and right is None:
                sg = sg_free(length)
            elif left is None or right is None:
                sg = sg_boundary(length)
            elif left == right:
                sg = sg_match(length)
            else:
                sg = sg_diff(length)

            xor_sum ^= sg
        else:
            i += 1

    # Decide winner from XOR
    sys.stdout.write("FIRST\n" if xor_sum != 0 else "SECOND\n")

if __name__ == "__main__":
    solve()
