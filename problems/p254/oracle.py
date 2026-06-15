import sys


def main():
    data = sys.stdin.read().split()
    N, Q = map(int, data)
    # Use 1-based indexing; index 0 unused
    nxt = [i + 1 for i in range(N + 1)]
    prv = [i - 1 for i in range(N + 1)]
    nxt[N] = 1
    prv[1] = N

    current = 1          # start at node 1
    clockwise = True     # initial direction
    remaining = N
    last_erased = -1

    # Loop until all nodes removed
    while remaining > 0:
        # Step 1: find Q-th node in current direction
        pos = current
        for _ in range(Q - 1):
            pos = nxt[pos] if clockwise else prv[pos]

        # Step 2: erase pos from the circle
        last_erased = pos
        left = prv[pos]
        right = nxt[pos]
        nxt[left] = right
        prv[right] = left
        remaining -= 1

        if remaining == 0:
            break

        # Step 3: move one step clockwise
        current = right

        # Step 4: update direction by parity of current
        clockwise = bool(current & 1)

    print(last_erased)


if __name__ == "__main__":
    main()
