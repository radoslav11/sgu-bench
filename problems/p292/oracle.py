# Read q: one side of the cemetery rectangle.
q = int(input())

# Read c: the other side of the cemetery rectangle.
c = int(input())

# Read n: grave length, each grave has size n x 1.
n = int(input())

# If both sides are at least n, we can use the full optimal formula.
if q >= n and c >= n:
    # s is the leftover part of q after full blocks of length n.
    s = q % n

    # t is the leftover part of c after full blocks of length n.
    t = c % n

    # Basic construction leaves an s * t uncovered corner.
    ans = (q * c - s * t) // n

    # If s + t > n, the pinwheel construction improves the packing.
    if s + t > n:
        ans += s + t - n

# Otherwise, one side is too short to fit a grave across it.
else:
    # The shorter side gives the number of independent rows/columns.
    short = min(q, c)

    # The longer side is where graves are placed.
    long = max(q, c)

    # Each of the short lines contains floor(long / n) graves.
    ans = short * (long // n)

# Output the maximum possible number of graves.
print(ans)
