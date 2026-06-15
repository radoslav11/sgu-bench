import sys

input = sys.stdin.readline


class Fenwick:
    def __init__(self, n):
        self.n = n
        self.f = [0] * (n + 1)

    def update(self, i, v):
        while i <= self.n:
            self.f[i] += v
            i += i & -i

    def query(self, i):
        s = 0
        while i > 0:
            s += self.f[i]
            i -= i & -i
        return s

    def lower_bound(self, target):
        pos = 0
        bit_mask = 1 << self.n.bit_length()
        while bit_mask:
            nxt = pos + bit_mask
            if nxt <= self.n and self.f[nxt] < target:
                target -= self.f[nxt]
                pos = nxt
            bit_mask >>= 1
        return pos + 1


CMAX = 10**6
bitCount = Fenwick(CMAX)
bitSum = Fenwick(CMAX)

out = []
for line in sys.stdin:
    parts = line.split()
    if not parts:
        continue
    op, a, b = parts[0], int(parts[1]), int(parts[2])
    if op == "ARRIVE":
        bitCount.update(b, a)
        bitSum.update(b, a * b)
    else:
        totalPieces = bitCount.query(CMAX)
        if totalPieces < a:
            out.append("UNHAPPY")
            continue
        p = bitCount.lower_bound(a)
        cntBelow = bitCount.query(p - 1)
        needAtP = a - cntBelow
        costBelow = bitSum.query(p - 1)
        totalCost = costBelow + needAtP * p
        if totalCost > b:
            out.append("UNHAPPY")
        else:
            rem = cntBelow
            while rem > 0:
                q = bitCount.lower_bound(rem)
                cntAtQ = bitCount.query(q) - bitCount.query(q - 1)
                r = min(cntAtQ, rem)
                bitCount.update(q, -r)
                bitSum.update(q, -r * q)
                rem -= r
            bitCount.update(p, -needAtP)
            bitSum.update(p, -needAtP * p)
            out.append("HAPPY")

print("\n".join(out))
