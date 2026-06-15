# Python handles arbitrarily large integers with the built-in int type.

import sys

def main():
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    # Read segment lengths as Python ints
    segs = list(map(int, data[1:]))

    # Sort the segments in non-decreasing order
    segs.sort()

    # Scan through all adjacent triples
    for i in range(n - 2):
        a, b, c = segs[i], segs[i+1], segs[i+2]
        # Check triangle condition for sorted a <= b <= c
        if a + b > c:
            # Print the first found triple and exit
            print(a, b, c)
            return

    # If none form a triangle, print three zeros
    print(0, 0, 0)

if __name__ == "__main__":
    main()
