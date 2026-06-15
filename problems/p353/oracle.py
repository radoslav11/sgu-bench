import sys


def max_talk_time(N, k1, k2, p1, p2, p3):
    if N < p1:
        return 0

    # p1 buys the first k1 seconds upfront; the call then survives a
    # second iff the balance is positive when the second starts, and is
    # dropped once the per-second charge makes it non-positive.
    bal = N - p1
    sec = k1
    if bal == 0:
        return sec

    if bal > k2 * p2:
        bal -= k2 * p2
        sec += k2
    else:
        t = bal // p2
        if bal % p2 == 0:
            return sec + t
        return sec + t + 1

    t = bal // p3
    if bal % p3 == 0:
        return sec + t
    return sec + t + 1


data = sys.stdin.read().split()
N, k1, k2, p1, p2, p3 = map(int, data)
print(max_talk_time(N, k1, k2, p1, p2, p3))
