import sys
sys.setrecursionlimit(10**7)
def read_input():
    data = sys.stdin.read().split()
    n, k = map(int, data[:2])
    s = data[2].strip()
    D = list(map(int, s))
    return n, k, D

def least_rotation(s):
    # Booth’s algorithm for minimum rotation in O(len(s))
    n = len(s)
    if n == 0:
        return 0
    # 'fail' array is like the prefix-function used in Booth’s
    fail = [-1] * (2*n)
    k = 0  # candidate start
    for j in range(1, 2*n):
        i = fail[j - k - 1]
        while i != -1 and s[j % n] != s[(k + i + 1) % n]:
            if s[j % n] < s[(k + i + 1) % n]:
                k = j - i - 1
            i = fail[i]
        if i == -1 and s[j % n] != s[(k + i + 1) % n]:
            if s[j % n] < s[(k + i + 1) % n]:
                k = j
            fail[j - k] = -1
        else:
            fail[j - k] = i + 1
    return k

def max_cyclic_shift(v):
    # To get maximum rotation, negate and find least rotation
    # Python ints are unbounded so negation works fine
    neg = [-x for x in v]
    start = least_rotation(neg)
    # build the rotated result
    return [v[(start + i) % len(v)] for i in range(len(v))]

def main():
    n, k, D = read_input()
    used = [False]*n
    best = None

    for i in range(n):
        if used[i]:
            continue
        # build one cycle
        cycle = []
        j = i
        while not used[j]:
            used[j] = True
            cycle.append(D[j])
            j = (j + k) % n
        # find its lexicographically maximum cyclic shift
        mx = max_cyclic_shift(cycle)
        if best is None or mx > best:
            best = mx

    # print first n digits of infinite repetition of 'best'
    L = len(best)
    out = ''.join(str(best[i % L]) for i in range(n))
    print(out)

if __name__ == "__main__":
    main()
