import sys
from collections import deque, defaultdict

def solve() -> None:
    input = sys.stdin.readline
    n, m = map(int, input().split())
    a = [list(map(int, input().split())) for _ in range(n)]

    # done[x][y] indicates the cell has been removed in the reverse process
    done = [[False] * m for _ in range(n)]

    # Values are 1..5. We maintain buckets of coordinates by current value.
    # In Python we won't try to do O(1) deletions from a linked list like in C++.
    # Instead, we use deques and "lazy deletion":
    # - when a cell's value changes, we append it to the new bucket
    # - when popping from a bucket, we discard entries that are outdated
    buckets = [deque() for _ in range(6)]
    for i in range(n):
        for j in range(m):
            buckets[a[i][j]].append((i, j))

    # Directions for 4-neighbors
    dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]

    # answer will store reverse order (last -> first in forward)
    answer = []

    # Helper to clean the front of a bucket:
    # remove entries that no longer have that value or are already done.
    def normalize_bucket(v: int) -> None:
        dq = buckets[v]
        while dq and (done[dq[0][0]][dq[0][1]] or a[dq[0][0]][dq[0][1]] != v):
            dq.popleft()

    # We keep processing while there exists at least one alive "1" cell.
    while True:
        normalize_bucket(1)
        if not buckets[1]:
            break

        # Collect all current alive 1-cells.
        # We must take all of them currently present; however with lazy deletion,
        # we iterate through the deque and filter by current validity.
        ones = []
        for (x, y) in list(buckets[1]):
            if not done[x][y] and a[x][y] == 1:
                ones.append((x, y))

        # Safety: if lazy structure got too many stale entries, that's fine,
        # normalize_bucket will remove them next time.

        # Check that no two adjacent alive cells are both 1.
        # If they were, removing one would decrement the other below 1.
        ones_set = set(ones)
        for x, y in ones:
            for dx, dy in dirs:
                nx, ny = x + dx, y + dy
                if 0 <= nx < n and 0 <= ny < m and not done[nx][ny]:
                    if (nx, ny) in ones_set:
                        sys.stdout.write("No solution\n")
                        return

        # Compute total decrements for each alive neighbor.
        decr = defaultdict(int)  # (nx,ny) -> count
        for x, y in ones:
            for dx, dy in dirs:
                nx, ny = x + dx, y + dy
                if 0 <= nx < n and 0 <= ny < m and not done[nx][ny]:
                    decr[(nx, ny)] += 1

        # Validate no cell drops below 1
        for (nx, ny), cnt in decr.items():
            if a[nx][ny] - cnt < 1:
                sys.stdout.write("No solution\n")
                return

        # Remove all ones (mark done and record)
        for x, y in ones:
            done[x][y] = True
            answer.append((x, y))

        # Apply decrements and push updated cells into their new buckets
        for (nx, ny), cnt in decr.items():
            a[nx][ny] -= cnt
            buckets[a[nx][ny]].append((nx, ny))

    # If not all cells were removed, some never became 1 in reverse => impossible
    if len(answer) != n * m:
        sys.stdout.write("No solution\n")
        return

    # Reverse to get forward order and print in 1-based coordinates
    answer.reverse()
    out = []
    for x, y in answer:
        out.append(f"{x+1} {y+1}")
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
