import sys
import math

def read_ints():
    return map(int, sys.stdin.readline().split())

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n, m = int(next(it)), int(next(it))

    wolves = []
    sheep = []

    # Helper to read one segment and turn into an angular interval
    def read_interval():
        x1, y1, x2, y2 = map(int, (next(it), next(it), next(it), next(it)))
        a = math.atan2(y1, x1)
        b = math.atan2(y2, x2)
        return (min(a, b), max(a, b))

    for _ in range(n):
        wolves.append(read_interval())
    for _ in range(m):
        sheep.append(read_interval())

    # Merge sheep intervals into non-overlapping forbidden zones
    sheep.sort()
    merged = []
    for interval in sheep:
        if not merged or interval[0] > merged[-1][1] + 1e-9:
            merged.append(list(interval))
        else:
            # Overlap: extend the end
            merged[-1][1] = max(merged[-1][1], interval[1])
    sheep = merged

    # Sort wolves by right endpoint for greedy stabbing
    wolves.sort(key=lambda x: x[1])

    last_shot = -1e18
    shots = 0
    pos_sheep = 0
    S = len(sheep)

    for wl, wr in wolves:
        # Already covered?
        if wl <= last_shot + 1e-9:
            continue

        # Skip sheep intervals that end before this wolf’s right edge
        while pos_sheep < S and sheep[pos_sheep][1] < wr:
            pos_sheep += 1

        # Best shot is at wr, but must avoid the next sheep interval
        shoot = wr
        if pos_sheep < S:
            s_left = sheep[pos_sheep][0]
            shoot = min(shoot, s_left - 1e-9)

        # If shoot < wl, no feasible shot
        if shoot < wl - 1e-9:
            print("No solution")
            return

        # Place shot
        last_shot = shoot
        shots += 1

    print(shots)

if __name__ == "__main__":
    main()
