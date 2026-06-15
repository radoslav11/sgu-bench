import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    intervals = []

    # Read all intervals
    for _ in range(n):
        A = int(next(it))
        B = int(next(it))
        intervals.append((A, B))

    # Sort by start A ascending
    intervals.sort(key=lambda x: x[0])

    Bmax = -10**18   # track the maximum end seen so far
    answer = 0       # count of redundant intervals

    # Sweep through intervals
    for A, B in intervals:
        # If this interval ends before Bmax, it's redundant
        if B < Bmax:
            answer += 1
        else:
            # Otherwise update Bmax
            Bmax = B

    # Print final count
    print(answer)

if __name__ == "__main__":
    main()
