import sys

data = sys.stdin.read().split()
it = iter(data)

n = int(next(it))
m = int(next(it))

deg = [0] * n
for _ in range(m):
    u = int(next(it)) - 1
    v = int(next(it)) - 1
    deg[u] += 1
    deg[v] += 1

pairs_sum = 0
for d in deg:
    pairs_sum += d * (d - 1)

result = pairs_sum + 2 * m
print(result)
