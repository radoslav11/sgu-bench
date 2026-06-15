import sys

def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])

    # Special case: N=1, no matches and thus 0 rounds; matrix is just [0].
    if n == 1:
        sys.stdout.write("0\n0\n")
        return

    # Minimal number of tours for complete graph K_n edge-coloring:
    # even n -> n-1, odd n -> n
    num_rounds = n - 1 if (n % 2 == 0) else n
    out_lines = [str(num_rounds)]

    # Size of the rotating "table":
    # odd n -> table has all n players
    # even n -> table has n-1 players, last player is "standing" and pairs with head each round
    table_size = n - 1 if (n % 2 == 0) else n

    # Initialize NxN schedule matrix with zeros
    schedule = [[0] * n for _ in range(n)]

    # For each round r (0-based internally; store r+1 in the matrix)
    for r in range(num_rounds):

        # Even case: pair the standing player (n-1) with the current "head" of the table
        if n % 2 == 0:
            head = (table_size - r) % table_size
            schedule[head][n - 1] = r + 1
            schedule[n - 1][head] = r + 1

        # Pair symmetric players on the table
        # There are (table_size-1)//2 pairs among table_size players each round
        half = (table_size - 1) // 2
        for i in range(1, half + 1):
            # Compute indices after rotation by r; Python % is already non-negative
            p1 = (i - r) % table_size
            p2 = (table_size - i - r) % table_size

            schedule[p1][p2] = r + 1
            schedule[p2][p1] = r + 1

    # Output the matrix with spaces
    for i in range(n):
        out_lines.append(" ".join(map(str, schedule[i])))

    sys.stdout.write("\n".join(out_lines) + "\n")

if __name__ == "__main__":
    main()
