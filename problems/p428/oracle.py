# We generate 1000 proper aligned addition rebuses using the same construction:
# - Each rebus consists of three 16-letter numbers L, R, S with
#   for i in 0..7:
#     column 2*i   : A + A = B        (buffer: ensures no carry)
#     column 2*i+1 : A + X = X+1      (order constraint; X ∈ A..H in a permuted order)
# This forces A=1, B=2, ..., I=9 uniquely (properness), and permuting the order
# of the 8 constraints yields pairwise non-isomorphic rebuses.

import sys

def next_permutation(seq):
    """
    In-place next lexicographic permutation on a list of comparable items.
    Returns True if the permutation was advanced, False if seq was the last permutation.
    """
    # 1) Find the largest i such that seq[i] < seq[i+1]
    i = len(seq) - 2
    while i >= 0 and seq[i] >= seq[i + 1]:
        i -= 1
    if i < 0:
        # Already the last permutation
        seq.reverse()
        return False

    # 2) Find the largest j > i such that seq[j] > seq[i]
    j = len(seq) - 1
    while seq[j] <= seq[i]:
        j -= 1

    # 3) Swap i and j
    seq[i], seq[j] = seq[j], seq[i]

    # 4) Reverse the suffix starting at i+1
    seq[i + 1:] = reversed(seq[i + 1:])
    return True

def solve():
    perm = list("ABCDEFGH")  # X ∈ {A..H}
    out = []
    for _ in range(1000):
        # Advance to the next permutation first (like the C++ code)
        next_permutation(perm)

        # Build the three aligned strings L, R, S (length 16)
        L_chars = []
        R_chars = []
        S_chars = []
        for i in range(8):
            # Buffer column: A + A = B
            L_chars.append('A')
            R_chars.append('A')
            S_chars.append('B')

            # Constraint column: A + X = X+1
            L_chars.append('A')
            R_chars.append(perm[i])
            S_chars.append(chr(ord(perm[i]) + 1))  # next letter

        L = "".join(L_chars)
        R = "".join(R_chars)
        S = "".join(S_chars)
        out.append(f"{L}+{R}={S}")

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
