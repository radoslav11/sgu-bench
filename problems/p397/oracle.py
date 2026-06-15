import sys
def main():
    data = sys.stdin.read().strip()
    # We'll treat 'left' and 'right' as stacks.
    left = []   # characters to the left of the cursor
    right = []  # characters to the right of the cursor

    for c in data:
        if c == 'L':
            if left:
                # Move one char from left to right (cursor moves left)
                right.append(left.pop())
        elif c == 'R':
            if right:
                # Move one char from right to left (cursor moves right)
                left.append(right.pop())
        else:
            # Letter insertion: push onto left (cursor moves right)
            left.append(c)

    # The final text is left + reversed(right)
    # right is in reverse order of actual text to its right
    sys.stdout.write(''.join(left) + ''.join(reversed(right)))

if __name__ == "__main__":
    main()
