import itertools

# Read input
l_val, r_val = map(int, input().split())

SENTINEL_NEXT = 10**18  # "no next permutation" marker


def is_inside(x: int) -> bool:
    """Check if x is inside [l_val, r_val]."""
    return l_val <= x <= r_val


def next_perm(x: int) -> int:
    """
    Return the next larger permutation of digits of x (same length),
    or SENTINEL_NEXT if none exists.
    """
    s = list(str(x))
    # Find next permutation in-place (classic algorithm).
    i = len(s) - 2
    while i >= 0 and s[i] >= s[i + 1]:
        i -= 1
    if i < 0:
        return SENTINEL_NEXT  # already the largest permutation

    j = len(s) - 1
    while s[j] <= s[i]:
        j -= 1

    s[i], s[j] = s[j], s[i]
    s[i + 1:] = reversed(s[i + 1:])
    return int("".join(s))


def prev_perm(x: int) -> int:
    """
    Return the previous smaller permutation of digits of x (same length),
    rejecting leading zero permutations. If none exists, return 0.
    """
    s = list(str(x))
    # Previous permutation algorithm.
    i = len(s) - 2
    while i >= 0 and s[i] <= s[i + 1]:
        i -= 1
    if i < 0:
        return 0  # already the smallest permutation

    j = len(s) - 1
    while s[j] >= s[i]:
        j -= 1

    s[i], s[j] = s[j], s[i]
    s[i + 1:] = reversed(s[i + 1:])

    if s[0] == '0':
        return 0  # invalid (would reduce number of digits)

    return int("".join(s))


def has_one_similar(x: int) -> bool:
    """
    Check whether x has exactly one other similar number in [l_val, r_val],
    using only prev/next permutation neighbors.
    """
    if not is_inside(x):
        return False

    n1 = next_perm(x)
    n2 = next_perm(n1) if n1 != SENTINEL_NEXT else SENTINEL_NEXT

    p1 = prev_perm(x)
    p2 = prev_perm(p1) if p1 != 0 else 0

    has_p1 = is_inside(p1)
    has_p2 = is_inside(p2)
    has_n1 = is_inside(n1)
    has_n2 = is_inside(n2)

    if has_p1:
        # Only prev1 is inside; no second below and none above.
        return (not has_p2) and (not has_n1)
    if has_n1:
        # Only next1 is inside; ensure next2 is outside.
        return not has_n2
    return False


def create_candidates(base: int, offset: int, out: list[int]) -> None:
    """
    Replicate the C++ candidate generation:
    from base and an offset (0..999), generate base, its neighbors,
    and numbers formed by taking base's prefix and attaching a suffix
    built from offset followed by repeated digits.
    """
    if base < l_val or base > r_val:
        return

    # base itself and its permutation neighbors
    out.append(base)

    nb = next_perm(base)
    if nb != SENTINEL_NEXT:
        out.append(nb)

    pb = prev_perm(base)
    if pb != 0:
        out.append(pb)

    for digit in range(10):
        suff = offset
        pow10 = 1000  # starts at 10^3

        for _len in range(15):
            # append digit to suffix
            suff = suff * 10 + digit
            pow10 *= 10

            if suff > r_val:
                break

            # keep the prefix of base, remove last digits corresponding to pow10
            pref = base - (base % pow10)
            extended = pref + suff

            out.append(extended)

            ne = next_perm(extended)
            if ne != SENTINEL_NEXT:
                out.append(ne)

            pe = prev_perm(extended)
            if pe != 0:
                out.append(pe)


# Generate candidates
candidates = []
for delta in range(1000):
    create_candidates(l_val, delta, candidates)
    create_candidates(r_val, delta, candidates)

# Unique candidates
candidates = sorted(set(candidates))

# Count those with exactly one similar in [l, r]
ans = 0
for x in candidates:
    if has_one_similar(x):
        ans += 1

print(ans)
