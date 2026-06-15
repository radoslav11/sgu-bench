import sys

sys.setrecursionlimit(50000)


def process(items, wrap_in_li):
    """
    items: list of strings to process at the current nesting level
    wrap_in_li:
      - False at top level: print plain lines directly
      - True inside an HTML list: every produced element must be wrapped in <li>..</li>
    """
    n = len(items)
    i = 0

    while i < n:
        c = items[i][0]

        # Attempt to form a list block if the line starts with '*' or '#'
        if c in ('*', '#'):
            j = i
            # Extend j over consecutive lines that start with the same marker
            while j < n and items[j] and items[j][0] == c:
                j += 1

            # If block size >= 2, it is a list per the statement
            if j - i >= 2:
                if wrap_in_li:
                    print("<li>")

                # Open correct list tag
                print("<ol>" if c == '#' else "<ul>")

                # Strip one marker and recurse; inside a list, elements must be <li>-wrapped
                stripped = [items[k][1:] for k in range(i, j)]
                process(stripped, True)

                # Close list tag
                print("</ol>" if c == '#' else "</ul>")

                if wrap_in_li:
                    print("</li>")

                i = j
                continue

        # Not a list block: output this single line as a plain item
        if wrap_in_li:
            print("<li>")
        print(items[i])
        if wrap_in_li:
            print("</li>")

        i += 1


def main():
    # Read all lines exactly (strip only the trailing newline)
    lines = [line.rstrip('\n') for line in sys.stdin]

    # Top level: do not wrap lines in <li>
    process(lines, False)

if __name__ == "__main__":
    main()
