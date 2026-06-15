import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    # First integer is N, followed by N sorted frequencies
    n = int(data[0])
    freqs = list(map(int, data[1:]))

    # q1: initial frequencies; q2: merged sums
    q1 = deque(freqs)
    q2 = deque()

    total_bits = 0

    # Continue until only one combined weight remains
    while len(q1) + len(q2) > 1:
        # Helper to pop the smallest front element from q1 or q2
        def pop_min():
            if not q2 or (q1 and q1[0] < q2[0]):
                return q1.popleft()
            else:
                return q2.popleft()

        # Take two smallest
        first = pop_min()
        second = pop_min()

        combined = first + second
        total_bits += combined
        q2.append(combined)

    # Print the result
    print(total_bits)

if __name__ == "__main__":
    main()
