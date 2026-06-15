import sys

def main():
    data = sys.stdin.read().strip().splitlines()
    it = iter(data)

    # Read dimensions
    n_m = next(it).split()
    n = int(n_m[0])
    m = int(n_m[1])

    # Read grid rows as strings of digits
    grid_strs = [next(it).strip() for _ in range(n)]

    # Convert to integer grid (0..9)
    grid = [[ord(ch) - ord('0') for ch in row] for row in grid_strs]

    # Read path
    path = next(it).strip()

    # Directions: 0:east, 1:south, 2:west, 3:north
    dx = [1, 0, -1, 0]
    dy = [0, 1, 0, -1]

    # Start at top-left intersection, facing east
    x, y = 0, 0
    dir = 0

    # visited[r][c] = number of times block (r,c) has been passed
    visited = [[0] * m for _ in range(n)]

    satisfaction = 0

    for c in path:
        if c == 'L':
            dir = (dir + 3) % 4  # turn left
        elif c == 'R':
            dir = (dir + 1) % 4  # turn right
        else:
            # Move one segment forward
            nx = x + dx[dir]
            ny = y + dy[dir]

            if dir == 0:
                # East: adjacent blocks above (y-1, x) and below (y, x)
                if y > 0 and x < m:
                    r, c0 = y - 1, x
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
                if y < n and x < m:
                    r, c0 = y, x
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
            elif dir == 1:
                # South: adjacent blocks left (y, x-1) and right (y, x)
                if y < n and x > 0:
                    r, c0 = y, x - 1
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
                if y < n and x < m:
                    r, c0 = y, x
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
            elif dir == 2:
                # West: adjacent blocks above (y-1, x-1) and below (y, x-1)
                if y > 0 and x > 0:
                    r, c0 = y - 1, x - 1
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
                if y < n and x > 0:
                    r, c0 = y, x - 1
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
            else:
                # North: adjacent blocks left (y-1, x-1) and right (y-1, x)
                if y > 0 and x > 0:
                    r, c0 = y - 1, x - 1
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1
                if y > 0 and x < m:
                    r, c0 = y - 1, x
                    beauty = grid[r][c0]
                    satisfaction += beauty if visited[r][c0] == 0 else beauty // 2
                    visited[r][c0] += 1

            # Advance to the next intersection
            x, y = nx, ny

    print(satisfaction)

if __name__ == "__main__":
    main()
