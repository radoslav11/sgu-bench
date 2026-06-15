import sys
from math import isqrt

# Read A and B from standard input
data = sys.stdin.read().strip().split()
A = int(data[0])
B = int(data[1])

def min_divisor(x: int) -> int:
    """
    Return the smallest non-trivial divisor of x (2 <= d < x).
    If x is prime, return -1.
    """
    # Try all integers d from 2 up to sqrt(x)
    # If any divides x, it's the smallest non-trivial divisor found.
    limit = isqrt(x)
    for d in range(2, limit + 1):
        if x % d == 0:
            return d
    # If we reached here, x has no non-trivial divisor: x is prime
    return -1

def solve_even(A: int, B: int):
    """
    Solve the problem for even A and even B.
    Return a list of integers representing a valid sequence
    from A to B (inclusive) under the rules.
    """
    assert A % 2 == 0 and B % 2 == 0

    pw2 = 2              # current power of two to try
    steps = []           # store the path from A to B

    # Continue while we haven't reached B
    while A < B:
        # Try doubling pw2 as long as:
        #  1) A + 2*pw2 <= B   (does not overshoot B)
        #  2) 2*pw2 < A        (remains a proper divisor)
        #  3) A % (2*pw2) == 0 (2*pw2 divides A)
        while A + 2 * pw2 <= B and 2 * pw2 < A and A % (2 * pw2) == 0:
            pw2 *= 2      # increase to the next power of two

        # Now ensure pw2 is not too big to add.
        # While A + pw2 > B, shrink pw2 by halving it.
        while A + pw2 > B:
            pw2 //= 2

        # Record current A as part of the sequence
        steps.append(A)
        # Perform the move A -> A + pw2
        A += pw2

    # Finally include B itself
    steps.append(B)
    return steps

def main():
    global A, B

    # Compute smallest non-trivial divisors of A and B
    d1 = min_divisor(A)
    dk = min_divisor(B)

    # Start with adjusted values equal to the originals
    adjusted_A = A
    adjusted_B = B

    # If A is odd, we want to first do A -> A + d1
    if A % 2 == 1:
        adjusted_A = A + d1

    # If B is odd, we want the last move to be
    # adjusted_B -> adjusted_B + dk = B, so adjusted_B = B - dk
    if B % 2 == 1:
        adjusted_B = B - dk

    # Check for impossibilities:
    # - If A is odd prime: d1 == -1 and we can't take the first step
    # - If B is odd prime: dk == -1 and we can't end at B
    # - If adjusted_A > adjusted_B: even interval can't be traversed
    if d1 == -1 or dk == -1 or adjusted_A > adjusted_B:
        print("Impossible")
        return

    # Now adjusted_A and adjusted_B are even and adjusted_A <= adjusted_B
    seq_even = solve_even(adjusted_A, adjusted_B)

    out_lines = []

    # If A was modified (odd), print original A first
    if adjusted_A != A:
        out_lines.append(str(A))

    # Then print the even sequence from adjusted_A to adjusted_B
    for x in seq_even:
        out_lines.append(str(x))

    # If B was modified (odd), finally print B
    if adjusted_B != B:
        out_lines.append(str(B))

    # Output result
    sys.stdout.write("\n".join(out_lines) + "\n")

if __name__ == "__main__":
    main()
