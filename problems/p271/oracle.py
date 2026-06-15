from sys import stdin
from collections import deque

def main():
    data = stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    K = int(next(it))

    # Read the initial pile, top at the front of this deque
    all_books = deque()
    for _ in range(n):
        all_books.append(next(it))

    # Split into prefix (first K) and suffix (rest)
    prefix = all_books
    suffix = deque()
    while len(prefix) > K:
        # Move the bottom of prefix to the top of suffix
        suffix.appendleft(prefix.pop())

    # rev=False means prefix is in "normal" order; True means logically reversed
    rev = False

    # Helpers to push_front and pop_back on prefix under rev-flag
    def prefix_push_front(book):
        # if not reversed, push to left; else push to right
        if not rev:
            prefix.appendleft(book)
        else:
            prefix.append(book)

    def prefix_pop_back():
        # if not reversed, pop rightmost; else pop leftmost
        return prefix.pop() if not rev else prefix.popleft()

    # Process operations
    for _ in range(m):
        op = next(it)
        if op.startswith("ADD"):
            # Extract name inside ADD(...)
            name = op[4:-1]
            # Insert at pile front => prefix front
            prefix_push_front(name)
            # If prefix got too big, move one book to suffix front
            if len(prefix) > K:
                moved = prefix_pop_back()
                suffix.appendleft(moved)

        else:  # ROTATE
            # Just toggle the reverse flag on prefix
            rev = not rev

    # Output the final pile: prefix then suffix
    out = []

    # Print prefix in correct order
    if rev:
        # If reversed, we iterate prefix from right to left
        for book in reversed(prefix):
            out.append(book)
    else:
        # Normal: left to right
        for book in prefix:
            out.append(book)

    # Then the suffix is always in normal order
    for book in suffix:
        out.append(book)

    # Print each on its own line
    print("\n".join(out))


if __name__ == "__main__":
    main()
