import sys


def distinct_substrings(s):
    # brute force: collect every substring into a set (s is short, O(sqrt n))
    subs = set()
    L = len(s)
    for i in range(L):
        for j in range(i + 1, L + 1):
            subs.add(s[i:j])
    return len(subs)


def better(a, b):
    # True if a is strictly preferable to b: shorter, or same length and lex smaller
    if b is None:
        return True
    if len(a) != len(b):
        return len(a) < len(b)
    return a < b


def main():
    n = int(sys.stdin.read().split()[0])

    # best[k] = lexicographically smallest shortest string with k distinct substrings,
    # built incrementally by inserting one character into best of a smaller count.
    best = [None] * (n + 1)
    best[1] = "a"
    if n >= 2:
        best[2] = "aa"
    if n >= 3:
        best[3] = "ab"
    if n >= 4:
        best[4] = "aaaa"

    for cnt in range(1, n):
        base = best[cnt]
        if base is None:
            continue
        max_c = chr(max(ord(ch) for ch in base) + 1)
        c = 'a'
        while c <= max_c:
            for pos in range(len(base) + 1):
                cand = base[:pos] + c + base[pos:]
                nc = distinct_substrings(cand)
                if nc > n:
                    continue
                if better(cand, best[nc]):
                    best[nc] = cand
            c = chr(ord(c) + 1)

    sys.stdout.write(best[n] + "\n")


if __name__ == "__main__":
    main()
