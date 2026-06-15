import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    a = [int(next(it)) for _ in range(n)]
    total = sum(a)
    # If no votes, we cannot form percentages
    if total == 0:
        print("No solution")
        return

    # Compute floor percentages and track how many points remain
    p = []
    current_sum = 0
    for votes in a:
        # floor of votes*100/total
        val = (votes * 100) // total
        p.append(val)
        current_sum += val

    # How many % points we still need to distribute
    need = 100 - current_sum

    # Collect indices with non-integer true percentage
    frac_indices = []
    for i, votes in enumerate(a):
        if (votes * 100) % total != 0:
            frac_indices.append(i)

    # If not enough non-integer entries to round up, no solution
    if need > len(frac_indices):
        print("No solution")
        return

    # Round up the first 'need' candidates with fractional percentage
    for i in range(need):
        idx = frac_indices[i]
        p[idx] += 1

    # Output result
    print(*p)

if __name__ == "__main__":
    main()
