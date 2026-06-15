"""Checker for p214: usage checker.py <in> <out> <ans>; exit 0 = accept.

The dissimilarity D is deterministic and must match the reference. The
certificate alpha, beta must be equal-length strings over Sigma of
length <= 4000 with lambda/mu as subsequences and total pairwise cost
exactly D. Any such pair is accepted; the reference's own strings are
never compared against.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def is_subsequence(small, big):
    it = iter(big)
    return all(c in it for c in small)


def main():
    in_lines = open(sys.argv[1]).read().split("\n")
    sigma, lam, mu = (in_lines[k].strip() for k in range(3))
    k = len(sigma)
    nums = " ".join(in_lines[3:]).split()
    idx = {c: i for i, c in enumerate(sigma)}
    cost = [[int(nums[i * k + j]) for j in range(k)] for i in range(k)]

    ans_tokens = open(sys.argv[3]).read().split()
    expected = int(ans_tokens[0])

    out_tokens = open(sys.argv[2]).read().split()
    if len(out_tokens) != 3:
        reject(f"expected 3 tokens (D, alpha, beta), got {len(out_tokens)}")
    try:
        d = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if d != expected:
        reject(f"dissimilarity is {expected}, got {d}")

    alpha, beta = out_tokens[1], out_tokens[2]
    if len(alpha) != len(beta):
        reject(f"alpha and beta lengths differ: {len(alpha)} vs {len(beta)}")
    if len(alpha) > 4000:
        reject(f"alpha length {len(alpha)} exceeds 4000")
    if any(c not in idx for c in alpha + beta):
        reject("alpha or beta uses a character outside Sigma")
    if not is_subsequence(lam, alpha):
        reject("lambda is not a subsequence of alpha")
    if not is_subsequence(mu, beta):
        reject("mu is not a subsequence of beta")

    total = sum(cost[idx[a]][idx[b]] for a, b in zip(alpha, beta))
    if total != expected:
        reject(f"dist(alpha, beta) = {total}, claimed optimum is {expected}")

    sys.exit(0)


if __name__ == "__main__":
    main()
