import sys

def main():
    # Read all tokens (integers) from stdin.
    data = sys.stdin.read().strip().split()
    if not data:
        return

    # Parse the first 16 integers as the puzzle (row-major order).
    vals = list(map(int, data[:16]))

    # Locate the blank (0).
    zero_index = vals.index(0)           # Index in [0..15]
    r, c = divmod(zero_index, 4)         # Row and column (0-based)

    # Manhattan distance from blank to the target position (3,3).
    d = (3 - r) + (3 - c)

    # Replace 0 with 16 (treat blank as largest tile) for inversion counting.
    vals[zero_index] = 16

    # Compute inversion count over all 16 values.
    inv = 0
    for i in range(16):
        for j in range(i + 1, 16):
            if vals[i] > vals[j]:
                inv += 1

    # Solvable iff (inv + d) is even.
    print("YES" if (inv + d) % 2 == 0 else "NO")

if __name__ == "__main__":
    main()
