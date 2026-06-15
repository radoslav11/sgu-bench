import sys
import threading

def main():
    import math
    input = sys.stdin.readline

    n = int(input())
    segments = [tuple(map(int, input().split())) for _ in range(n)]

    # Map: point (x,y) -> dict of direction (dx,dy) -> count
    mp = {}

    def normalize(dx, dy):
        """Return a canonical primitive direction for vector (dx,dy)."""
        g = math.gcd(dx, dy)
        dx //= g
        dy //= g
        # Ensure dx >= 0, or if dx==0 then dy>0
        if dx < 0:
            dx, dy = -dx, -dy
        if dx == 0:
            # vertical line: force dy = +1
            dy = 1
        if dy == 0:
            # horizontal line: force dx = +1
            dx = 1
        return dx, dy

    # Build the mapping
    for x1, y1, x2, y2 in segments:
        dx, dy = x2 - x1, y2 - y1
        dirn = normalize(dx, dy)
        for pt in ((x1,y1), (x2,y2)):
            if pt not in mp:
                mp[pt] = {}
            mp[pt][dirn] = mp[pt].get(dirn, 0) + 1

    ans = 0
    # For each shared endpoint, count perpendicular pairs
    for slopes in mp.values():
        for (dx, dy), cnt in slopes.items():
            # perpendicular vector
            pdx, pdy = -dy, dx
            pdx, pdy = normalize(pdx, pdy)
            # add cross count if exists
            cnt2 = slopes.get((pdx, pdy), 0)
            ans += cnt * cnt2

    # Every pair was counted twice (A with B and B with A)
    print(ans // 2)

if __name__ == "__main__":
    threading.Thread(target=main).start()
