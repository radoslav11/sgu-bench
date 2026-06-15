import sys

def moves_to_unify(a, b):
    """
    Return the minimum number of moves to gather all balls in one box
    under the rule: transfer from one box to the other exactly as many
    balls as are currently in the destination box. Return -1 if impossible.
    """
    for steps in range(61):
        # Always make 'a' the smaller or equal box
        if a > b:
            a, b = b, a
        # If the smaller box is empty, we're done
        if a == 0:
            return steps
        # Move from b to a: b loses a, a doubles
        b -= a
        a <<= 1  # equivalent to a *= 2
    # If we reach here, no solution within 60 moves → impossible
    return -1

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    A, B = map(int, data[:2])
    result = moves_to_unify(A, B)
    print(result)

if __name__ == "__main__":
    main()
