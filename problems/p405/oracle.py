import sys

def main():
    data = list(map(int, sys.stdin.read().split()))  # Read all integers from stdin.
    it = iter(data)                                   # Create an iterator for sequential reading.

    try:
        n = next(it)                                  # Number of participants.
        m = next(it)                                  # Number of matches.
    except StopIteration:
        return                                        # Empty input; nothing to do.

    # Initialize total scores for all participants.
    scores = [0] * n

    # Process each match.
    for _ in range(m):
        # Read the actual result (a, b).
        a = next(it)
        b = next(it)

        # For each participant, read prediction and compute score contribution.
        for i in range(n):
            p = next(it)
            q = next(it)

            add = 0

            # Outcome correct (winner or draw)?
            if (a > b and p > q) or (a < b and p < q) or (a == b and p == q):
                add += 2

            # Difference correct?
            if (a - b) == (p - q):
                add += 3

            # First team's goals correct?
            if a == p:
                add += 1

            # Second team's goals correct?
            if b == q:
                add += 1

            scores[i] += add

    # Output the scores separated by spaces.
    print(' '.join(map(str, scores)))

if __name__ == "__main__":
    main()
