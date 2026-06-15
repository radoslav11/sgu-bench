import sys

INF = float("inf")


def solve():
    data = sys.stdin.read().split()
    m, n = int(data[0]), int(data[1])
    cake = data[2:2 + m]

    # Cell labels in the sliding window of the last n processed cells:
    # 0 = empty, 1 = solid (candle / covered), 2 = V-top (needs the cell
    # below), 3 = H-left (needs the cell to the right, only ever newest).
    dp = {(): 0}
    window_len = 0

    for i in range(m):
        for j in range(n):
            ndp = {}
            blocked = cake[i][j] == '*'
            for state, cost in dp.items():
                above = state[0] if (i > 0 and window_len == n) else None
                left = state[-1] if (j > 0 and window_len >= 1) else None

                def push(label, add):
                    body = state[1:] if window_len == n else state
                    if left == 3:
                        body = body[:-1] + (1,)
                    ns = body + (label,)
                    val = cost + add
                    if val < ndp.get(ns, INF):
                        ndp[ns] = val

                if above == 2:
                    # Forced V-bottom; clashes with pending H or a candle.
                    if not blocked and left != 3:
                        push(1, 0)
                    continue
                if left == 3:
                    # Forced H-right.
                    if not blocked:
                        push(1, 0)
                    continue
                if blocked:
                    push(1, 0)
                    continue
                # Leave empty if no empty neighbour above/left.
                if above != 0 and left != 0:
                    push(0, 0)
                # Start a vertical domino.
                if i + 1 < m:
                    push(2, 1)
                # Start a horizontal domino.
                if j + 1 < n:
                    push(3, 1)

            dp = ndp
            window_len = min(window_len + 1, n)

    ans = min(
        (cost for state, cost in dp.items() if 2 not in state and 3 not in state),
        default=INF,
    )
    print(ans)


if __name__ == "__main__":
    solve()
