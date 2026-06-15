import sys
import math

def main():
    # Read the first integer from input (robust to extra whitespace/newlines)
    data = sys.stdin.read().strip().split()
    if not data:
        return
    L = int(data[0])  # L = 2N - 1 (odd per problem statement)

    # Compute N = (L + 1) // 2 (since L = 2N - 1)
    N = (L + 1) // 2

    # The key "step" that characterizes beauty is d = N + 1.
    d = N + 1

    # The graph with edges i -- (i + d mod L) decomposes into g cycles.
    g = math.gcd(L, d)

    # Each cycle of length len = L // g has maximum independent set floor(len/2).
    # So the maximum number of black beads that still avoids beauty is:
    max_avoid = g * ((L // g) // 2)

    # Minimal K that forces beauty in every coloring:
    ans = max_avoid + 1

    print(ans)

if __name__ == "__main__":
    main()
