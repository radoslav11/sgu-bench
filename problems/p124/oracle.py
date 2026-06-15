import sys


def main():
    data = sys.stdin.read().split()
    k = int(data[0])
    segs = []
    pos = 1
    for _ in range(k):
        x1, y1, x2, y2 = (int(v) for v in data[pos:pos + 4])
        pos += 4
        segs.append((x1, y1, x2, y2))
    x0, y0 = int(data[pos]), int(data[pos + 1])

    for x1, y1, x2, y2 in segs:
        if x1 == x2:
            if x0 == x1 and min(y1, y2) <= y0 <= max(y1, y2):
                print("BORDER")
                return
        else:
            if y0 == y1 and min(x1, x2) <= x0 <= max(x1, x2):
                print("BORDER")
                return

    # Cast a ray to the right: count vertical segments strictly right of
    # the point whose half-open y-range [ylo, yhi) covers y0.
    crossings = 0
    for x1, y1, x2, y2 in segs:
        if x1 == x2 and x1 > x0:
            ylo, yhi = min(y1, y2), max(y1, y2)
            if ylo <= y0 < yhi:
                crossings ^= 1

    print("INSIDE" if crossings else "OUTSIDE")


if __name__ == "__main__":
    main()
