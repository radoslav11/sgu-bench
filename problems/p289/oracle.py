import sys
sys.setrecursionlimit(1000000)

STATES = 3 ** 9  # 19683

# Winning triples in flattened 0..8 indexing
LINES = [
    (0, 1, 2), (3, 4, 5), (6, 7, 8),  # rows
    (0, 3, 6), (1, 4, 7), (2, 5, 8),  # cols
    (0, 4, 8), (2, 4, 6)              # diagonals
]

# dp[state] = 1  (X wins), 0 (draw), -1 (0 wins)
dp = [0] * STATES

# reachable[state] = whether this state can occur from empty board via legal play
reachable = [False] * STATES

# Precompute powers of 3 for fast digit operations
pow3 = [1] * 9
for i in range(1, 9):
    pow3[i] = pow3[i - 1] * 3

def get_cell(state: int, i: int) -> int:
    """Return base-3 digit at position i: 0 empty, 1 X, 2 0."""
    return (state // pow3[i]) % 3

def set_cell(state: int, i: int, v: int) -> int:
    """
    Return new state with digit i set to v by adding v * 3^i.
    Assumes digit i is currently 0.
    """
    return state + v * pow3[i]

def check_winner(state: int) -> int:
    """Return 1 if X wins, 2 if 0 wins, 0 otherwise."""
    for a, b, c in LINES:
        va = get_cell(state, a)
        if va != 0 and va == get_cell(state, b) and va == get_cell(state, c):
            return va
    return 0

def count_pieces(state: int) -> int:
    """Count number of non-empty cells."""
    cnt = 0
    for i in range(9):
        if get_cell(state, i) != 0:
            cnt += 1
    return cnt

def precompute():
    """DFS from empty board to mark reachable states and compute minimax dp."""
    reachable[0] = True

    def dfs(state: int):
        w = check_winner(state)
        if w != 0:
            # Terminal: someone already won
            dp[state] = 1 if w == 1 else -1
            return

        cnt = count_pieces(state)
        if cnt == 9:
            # Terminal: full board => draw
            dp[state] = 0
            return

        # Determine player to move by parity of pieces
        player = 1 if (cnt % 2 == 0) else 2  # 1=X, 2=0

        if player == 1:
            # X chooses the move with maximum dp outcome
            best = -2
            for i in range(9):
                if get_cell(state, i) == 0:
                    nxt = set_cell(state, i, 1)
                    if not reachable[nxt]:
                        reachable[nxt] = True
                        dfs(nxt)
                    best = max(best, dp[nxt])
            dp[state] = best
        else:
            # 0 chooses the move with minimum dp outcome
            best = 2
            for i in range(9):
                if get_cell(state, i) == 0:
                    nxt = set_cell(state, i, 2)
                    if not reachable[nxt]:
                        reachable[nxt] = True
                        dfs(nxt)
                    best = min(best, dp[nxt])
            dp[state] = best

    dfs(0)

def encode_board(rows) -> int:
    """Convert 3 strings of length 3 into base-3 encoded state."""
    state = 0
    for i in range(9):
        ch = rows[i // 3][i % 3]
        if ch == 'X':
            state = set_cell(state, i, 1)
        elif ch == '0':
            state = set_cell(state, i, 2)
    return state

def main():
    precompute()

    data = sys.stdin.read().splitlines()
    out = []
    i = 0
    while i < len(data):
        line = data[i].strip()
        i += 1

        if line == "Qc":
            break

        # Each test case: 3 lines of board, then an empty line (may be absent at EOF)
        row0 = line
        row1 = data[i].strip(); i += 1
        row2 = data[i].strip(); i += 1

        # Optional empty line separator
        if i < len(data) and data[i].strip() == "":
            i += 1

        state = encode_board([row0, row1, row2])

        if not reachable[state]:
            out.append("Illegal position.")
        else:
            if dp[state] == 1:
                out.append("X wins.")
            elif dp[state] == -1:
                out.append("0 wins.")
            else:
                out.append("Game is a draw.")

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
