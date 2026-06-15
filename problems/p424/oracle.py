import sys

# We will implement the same logic as in the C++ solution:
# Given n (1 <= n <= 100), construct a beautiful graph with
# the maximum possible number of edges.

def main():
    data = sys.stdin.read().strip()
    # Input is a single integer n.
    n = int(data)

    # Handle small cases directly based on problem statement / examples.
    if n == 1:
        # Single vertex, no edges.
        # Output: "1 0"
        print("1 0")
        return

    if n == 2:
        # Two vertices, one edge between them, no cycles.
        print("2 1")
        print("1 2")
        return

    if n == 3:
        # Three vertices, use a complete graph K3 (a triangle).
        print("3 3")
        print("1 2")
        print("2 3")
        print("1 3")
        return

    # For n >= 4:
    # Construction:
    #   Vertices: 1..n
    #   For each i in 1..n-2, add edges:
    #       (i, n-1) and (i, n)
    #
    # This creates 2 * (n-2) edges total.
    # All cycles are 4-cycles using both centers n-1 and n.
    # Any two cycles share 0 or 2 edges, never exactly 1.
    V = n
    E = 2 * (n - 2)

    # First line: number of vertices and edges.
    print(V, E)

    # Next E lines: each edge "u v".
    for i in range(1, n - 1):      # i runs 1 .. n-2
        # Edge between i and center n-1
        print(i, n - 1)
        # Edge between i and center n
        print(i, n)

if __name__ == "__main__":
    main()
