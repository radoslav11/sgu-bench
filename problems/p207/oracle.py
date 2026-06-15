import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    y = int(next(it))
    x = [int(next(it)) for _ in range(n)]

    # Compute floor, fractional parts, and prepare for sorting
    floors = [0]*n
    fracs  = [0.0]*n
    sum_floor = 0

    for i in range(n):
        # ideal real share times M
        exact = x[i]*m / y
        f = int(exact)           # floor
        sum_floor += f
        floors[i] = f
        fracs[i] = exact - f     # fractional part in [0,1)

    # buffer = how many 1-coin increments we need
    buffer = m - sum_floor

    # Pair each index with its fractional part
    # We want the largest fractional parts first
    idx = list(range(n))
    idx.sort(key=lambda i: fracs[i], reverse=True)

    # Promote the top 'buffer' floors to floors+1
    for k in range(buffer):
        i = idx[k]
        floors[i] += 1

    # Output the result
    print(*floors)

if __name__ == "__main__":
    main()
