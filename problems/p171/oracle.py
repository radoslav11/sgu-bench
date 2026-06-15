import sys
def main():
    data = sys.stdin.read().split()
    it = iter(data)
    K = int(next(it))

    # Read capacities and compute total number of students
    caps = [int(next(it)) for _ in range(K)]
    totalN = sum(caps)

    # Read zone qualities
    qs = [int(next(it)) for _ in range(K)]

    # Read student skills and weights
    ps = [int(next(it)) for _ in range(totalN)]
    ws = [int(next(it)) for _ in range(totalN)]

    # Build zone objects: (quality, remaining capacity, original index)
    zones = sorted([ [qs[i], caps[i], i] for i in range(K) ], key=lambda z: z[0])

    # Build student objects: (weight, skill, original index)
    students = sorted([ [ws[i], ps[i], i] for i in range(totalN) ],
                      key=lambda s: -s[0])  # descending weight

    # Placeholder for answers
    answer = [-1]*totalN

    # Greedy assignment: heavy students first
    for w, p, sid in students:
        # scan zones from highest quality down
        for j in range(K-1, -1, -1):
            q, cap, zid = zones[j]
            if q < p and cap > 0:
                answer[sid] = zid + 1
                zones[j][1] -= 1  # decrease capacity
                break

    # Fill leftovers
    zptr = 0
    for i in range(totalN):
        if answer[i] == -1:
            # find next zone with capacity
            while zones[zptr][1] == 0:
                zptr += 1
            answer[i] = zones[zptr][2] + 1
            zones[zptr][1] -= 1

    # Print results
    print(" ".join(map(str, answer)))

if __name__ == "__main__":
    main()

