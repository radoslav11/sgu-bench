import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    # Read T_i and L_i
    T = [int(next(it)) for _ in range(n)]
    L = [int(next(it)) for _ in range(n)]
    # Pair them as (T_i, L_i)
    jobs = list(zip(T, L))
    # Sort by the two-job comparator:
    # For speed, we can just sort descending by L_i,
    # which is equivalent to the pairwise rule.
    jobs.sort(key=lambda x: -x[1])

    current_time = 0  # cumulative printing time
    answer = 0        # maximum C_i + L_i

    for t, l in jobs:
        current_time += t       # finish printing this job
        answer = max(answer, current_time + l)
    print(answer)

if __name__ == "__main__":
    main()
