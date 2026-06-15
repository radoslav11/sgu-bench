import sys

def main():
    # Read all integers from stdin; input format is:
    # Line 1: N M
    # Line 2: is js
    data = list(map(int, sys.stdin.read().strip().split()))
    if len(data) < 4:
        return
    N, M, is_, js_ = data[0], data[1], data[2], data[3]

    # Use 1-based indexing to mirror the statement and make bounds checks cleaner.
    destroyed = [[False] * (M + 1) for _ in range(N + 1)]

    # Current robot position.
    i, j = is_, js_

    # Result string builder (list for efficiency, then join).
    res = []

    # Direction vectors and corresponding letters in fixed order DLUR.
    di = [1, 0, -1, 0]
    dj = [0, -1, 0, 1]
    letters = ['D', 'L', 'U', 'R']

    while True:
        # Mark the current cell as destroyed (it explodes when leaving).
        destroyed[i][j] = True

        moved = False  # Track whether we can move to any neighbor.

        # Try to move in DLUR order.
        for k in range(4):
            ni = i + di[k]
            nj = j + dj[k]
            # Check the neighbor is inside the grid and not yet destroyed.
            if 1 <= ni <= N and 1 <= nj <= M and not destroyed[ni][nj]:
                res.append(letters[k])  # Record the move.
                i, j = ni, nj           # Move to the neighbor.
                moved = True
                break

        # If no move was possible, stop the simulation.
        if not moved:
            break

    # Print the concatenated sequence of moves.
    print(''.join(res))

if __name__ == "__main__":
    main()
