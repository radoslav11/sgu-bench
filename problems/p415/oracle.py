import sys


def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    x = int(next(it))
    a = [int(next(it)) for _ in range(n)]

    # dp_pref[i] : bitmask of achievable sums using a[0..i-1]
    dp_pref = [0] * (n + 2)
    dp_suf = [0] * (n + 2)

    dp_pref[0] = 1
    dp_suf[n + 1] = 1

    full_mask = (1 << (x + 1)) - 1

    for i in range(1, n + 1):
        prev = dp_pref[i - 1]
        dp_pref[i] = prev | ((prev << a[i - 1]) & full_mask)

    for i in range(n, 0, -1):
        nxt = dp_suf[i + 1]
        dp_suf[i] = nxt | ((nxt << a[i - 1]) & full_mask)

    necessary = []
    for i in range(1, n + 1):
        pref = dp_pref[i - 1]
        # sums t reachable after i, reversed so bit j means t = x - j
        suf = dp_suf[i + 1]
        rev = int(bin(suf | (1 << (x + 1)))[3:][::-1], 2)
        if pref & rev == 0:
            necessary.append(a[i - 1])

    print(len(necessary))
    print(" ".join(map(str, necessary)))


if __name__ == "__main__":
    main()
