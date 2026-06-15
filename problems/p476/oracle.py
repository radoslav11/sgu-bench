import sys


def main():
    sys.set_int_max_str_digits(1000000)
    data = sys.stdin.read().split()
    idx = 0
    n = int(data[idx]); idx += 1
    k = int(data[idx]); idx += 1
    triples = []
    for _ in range(k):
        a = int(data[idx]); b = int(data[idx + 1]); c = int(data[idx + 2])
        idx += 3
        triples.append((a, b, c))

    m = 3 * n
    factorial = [1] * (m + 1)
    for i in range(1, m + 1):
        factorial[i] = factorial[i - 1] * i

    # ways(t): number of ways to split 3*t labelled students into t unordered
    # teams of three = (3t)! / (6^t * t!).
    def ways(t):
        return factorial[3 * t] // ((6 ** t) * factorial[t])

    # Map the students that appear in triples to bit positions so disjointness
    # is one integer AND. Inclusion-exclusion: a vertex-disjoint subset of s
    # forbidden triples contributes (-1)^s * ways(n - s). Enumerate disjoint
    # subsets by recursion that only branches into a triple when it does not
    # collide with the ones already chosen.
    bit = {}
    masks = []
    for a, b, c in triples:
        mk = 0
        for v in (a, b, c):
            if v not in bit:
                bit[v] = len(bit)
            mk |= 1 << bit[v]
        masks.append(mk)

    # coef[s] = signed count of vertex-disjoint subsets of size s.
    coef = [0] * (k + 1)

    def rec(pos, used, size):
        if pos == k:
            coef[size] += 1
            return
        rec(pos + 1, used, size)
        if used & masks[pos] == 0:
            rec(pos + 1, used | masks[pos], size + 1)

    rec(0, 0, 0)

    ans = 0
    for s in range(k + 1):
        if coef[s]:
            sign = -1 if (s & 1) else 1
            ans += sign * coef[s] * ways(n - s)

    print(ans)


if __name__ == "__main__":
    main()
