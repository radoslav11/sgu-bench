import sys

data = sys.stdin.buffer.read().split()
n = int(data[0])
m = int(data[1])

a = list(range(1, n + 1))
for i in range(m):
    p = int(data[2 + 2 * i])
    q = int(data[3 + 2 * i])
    a[p - 1:q] = a[p - 1:q][::-1]

sys.stdout.write(" ".join(map(str, a)) + "\n")
