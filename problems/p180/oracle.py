import sys
data = sys.stdin.read().strip().split()
n = int(data[0])
A = list(map(int, data[1:]))

vals = sorted(set(A))
rank = {v: i + 1 for i, v in enumerate(vals)}
M = len(vals)


class Fenwick:
    def __init__(self, size):
        self.n = size
        self.fw = [0] * (size + 1)

    def add(self, i, v):
        while i <= self.n:
            self.fw[i] += v
            i += i & -i

    def sum(self, i):
        s = 0
        while i > 0:
            s += self.fw[i]
            i -= i & -i
        return s


fenw = Fenwick(M)
inversions = 0

for i, x in enumerate(A):
    r = rank[x]
    smaller_or_equal = fenw.sum(r)
    inversions += (i - smaller_or_equal)
    fenw.add(r, 1)

print(inversions)
