import sys


def normalize(cells):
    """
    Normalize a shape.

    Given a list of cells [(r, c), ...], shift all cells so that
    the minimum row and minimum column become 0.

    The result is returned as a sorted tuple of pairs so it can be:
    - compared lexicographically,
    - used as a dictionary key.
    """

    # Find the topmost row used by the shape.
    min_r = min(r for r, c in cells)

    # Find the leftmost column used by the shape.
    min_c = min(c for r, c in cells)

    # Shift every cell by subtracting the minimum row and column.
    shifted = [(r - min_r, c - min_c) for r, c in cells]

    # Sort to get a deterministic representation.
    shifted.sort()

    # Return tuple because lists are not hashable in Python.
    return tuple(shifted)


def rotate(cells):
    """
    Rotate a shape by 90 degrees.

    Rotation formula:
        (r, c) -> (c, -r)

    After rotation, normalize the result.
    """

    # Apply the rotation transformation to every cell.
    rotated = [(c, -r) for r, c in cells]

    # Normalize the rotated shape before returning it.
    return normalize(rotated)


def get_orientations(cells):
    """
    Generate all distinct rotations of a shape.

    A piece may be rotated by:
    - 0 degrees,
    - 90 degrees,
    - 180 degrees,
    - 270 degrees.

    Symmetric pieces may have duplicate rotations, so duplicates are removed.
    """

    # Start from normalized original shape.
    cur = normalize(cells)

    # Stores unique orientations.
    orientations = []

    # Generate four rotations.
    for _ in range(4):
        # Add only if this orientation has not appeared before.
        if cur not in orientations:
            orientations.append(cur)

        # Rotate for the next iteration.
        cur = rotate(cur)

    # Return unique rotations.
    return orientations


def solve():
    # Read all input lines.
    data = sys.stdin.read().splitlines()

    # First line is N.
    n = int(data[0])

    # Next 20 lines describe the table.
    grid = data[1:21]

    # Dictionary from piece letter to list of occupied cells.
    piece_cells = {}

    # Scan the 20 x 20 table.
    for i in range(20):
        for j in range(20):
            ch = grid[i][j]

            # Dot means empty table cell.
            if ch == ".":
                continue

            # Add this coordinate to the corresponding piece.
            piece_cells.setdefault(ch, []).append((i, j))

    # Groups identical pieces up to rotation.
    #
    # Key:
    #   canonical shape representation.
    #
    # Value:
    #   dictionary containing:
    #   - "orients": list of unique orientations,
    #   - "letters": unused letters of this shape.
    grouped = {}

    # Process pieces in sorted letter order for deterministic output.
    for letter in sorted(piece_cells):
        # Generate all orientations of this piece.
        orients = get_orientations(piece_cells[letter])

        # Canonical key is the lexicographically smallest orientation.
        key = min(orients)

        # Create group if not present.
        if key not in grouped:
            grouped[key] = {
                "orients": orients,
                "letters": []
            }

        # Add this piece letter to its group.
        grouped[key]["letters"].append(letter)

    # Convert groups to a list for iteration in backtracking.
    groups = list(grouped.values())

    # Current result board, initially empty.
    board = [["." for _ in range(n)] for _ in range(n)]

    def backtrack():
        """
        Recursive exact-cover search.

        Finds the first empty cell and tries to place one remaining piece
        so that its first normalized cell lands there.
        """

        # First empty cell coordinates.
        target_r = -1
        target_c = -1

        # Find first empty cell in row-major order.
        for i in range(n):
            found = False
            for j in range(n):
                if board[i][j] == ".":
                    target_r = i
                    target_c = j
                    found = True
                    break
            if found:
                break

        # If no empty cell exists, the board is completely tiled.
        if target_r == -1:
            return True

        # Try every group of identical shapes.
        for group in groups:
            # If no unused pieces of this shape remain, skip.
            if not group["letters"]:
                continue

            # Try each possible rotation of this shape.
            for orient in group["orients"]:
                # First cell of this normalized orientation.
                anchor_r, anchor_c = orient[0]

                # Actual cells this placement would occupy.
                placed_cells = []

                # Assume valid until proven otherwise.
                ok = True

                # Check every cell of the piece.
                for r, c in orient:
                    # Translate this cell so anchor goes to target.
                    nr = target_r + r - anchor_r
                    nc = target_c + c - anchor_c

                    # Check board bounds.
                    if nr < 0 or nr >= n or nc < 0 or nc >= n:
                        ok = False
                        break

                    # Check overlap.
                    if board[nr][nc] != ".":
                        ok = False
                        break

                    # Remember cell for placement if valid.
                    placed_cells.append((nr, nc))

                # If this orientation cannot be placed here, try another.
                if not ok:
                    continue

                # Choose one unused letter from this shape group.
                letter = group["letters"].pop()

                # Place the piece.
                for r, c in placed_cells:
                    board[r][c] = letter

                # Continue recursively.
                if backtrack():
                    return True

                # Undo placement.
                for r, c in placed_cells:
                    board[r][c] = "."

                # Restore the letter.
                group["letters"].append(letter)

        # No option worked.
        return False

    # A solution is guaranteed, so this should succeed.
    backtrack()

    # Print the constructed board.
    print("\n".join("".join(row) for row in board))


if __name__ == "__main__":
    solve()
