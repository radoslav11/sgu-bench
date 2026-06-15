import sys

def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    # Read grid size
    n = int(next(it))   # height
    m = int(next(it))   # width

    # Read positions (row, col)
    cx = int(next(it)); cy = int(next(it))
    tx = int(next(it)); ty = int(next(it))

    first = next(it)    # 'C' or 'T'

    # --- Symmetry reduction 1: ensure |dx| >= |dy| by swapping axes if needed ---
    if abs(cx - tx) < abs(cy - ty):
        # swap x<->y for both players
        cx, cy = cy, cx
        tx, ty = ty, tx
        # swap dimensions accordingly
        n, m = m, n

    # --- Symmetry reduction 2: ensure cop is not below thief (cx <= tx) ---
    if cx > tx:
        # reflect vertically: x -> n+1-x
        cx = (n + 1) - cx
        tx = (n + 1) - tx

    ans = 0

    # If cop moves first, the solution assumes optimal is to move +1 in x direction
    # (downwards in normalized coordinates), thus consuming move #1.
    if first == 'C':
        cx += 1
        ans += 1

    # If capture already happened
    if cx == tx and cy == ty:
        sys.stdout.write(str(ans) + "\n")
        return

    # --- 1D case: width m == 1 (a single column path) ---
    if m == 1:
        # Base chase time (each step effectively costs 2 moves: thief moves in between)
        ans += (n - cx - 1) * 2
        # Parity adjustment because staying put is forbidden
        ans += ((cx ^ tx) & 1)
        sys.stdout.write(str(ans) + "\n")
        return

    # --- 2D case: if opposite chessboard colors, thief can evade forever ---
    # Parity of (x+y) mod2 equals parity of (x xor y) & 1
    if (((cx ^ cy) ^ (tx ^ ty)) & 1) == 1:
        sys.stdout.write("0\n")
        return

    # --- Same color: cop can force capture. Use closed-form formulas. ---
    if abs(cx - tx) == abs(cy - ty):
        # Normalize so cop is not left of thief: ensure cy >= ty by reflecting horizontally
        if cy < ty:
            cy = (m + 1) - cy
            ty = (m + 1) - ty

        # Diagonal-case formula, multiplied by 2 for alternating turns
        ans += (cy - cx + n - 2) * 2
        sys.stdout.write(str(ans) + "\n")
        return

    # General case formula
    ans += (n - cx - 1 + max(cy - 1, m - cy)) * 2
    sys.stdout.write(str(ans) + "\n")


if __name__ == "__main__":
    solve()
