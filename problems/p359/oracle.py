import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    lines = data[1:1+n]

    # Each "cell" is modeled as a one-element list [value],
    # because lists are mutable and can be shared (aliasing) easily.
    cell_a = [0]
    cell_b = [0]
    cell_c = [0]
    cell_d = [0]

    # Pointer variables map to the cell object they currently point to.
    ptr = {
        'a': cell_a,
        'b': cell_b,
        'c': cell_c,
        'd': cell_d,
    }

    out = []

    for line in lines:
        # Remove trailing semicolon
        if line.endswith(';'):
            line = line[:-1]

        # Handle print statement: writeln(x^)
        if line.startswith("writeln("):
            inside = line[len("writeln("):]
            # remove trailing ')'
            if inside.endswith(')'):
                inside = inside[:-1]

            # inside should be like "a^"
            x = inside[0]            # pointer name
            # dereference: the integer value is in ptr[x][0]
            out.append(str(ptr[x][0]))
            continue

        # Otherwise assignment, split by ':='
        left, right = line.split(":=")

        # If left is like "a^", we assign to the pointed value
        if left.endswith('^'):
            x = left[0]              # destination pointer variable

            # Case: constant digit
            if len(right) == 1 and right.isdigit():
                ptr[x][0] = int(right)
            else:
                # Case: y^ (copy value)
                y = right[0]
                ptr[x][0] = ptr[y][0]

        else:
            # Pointer assignment: x := y  (alias pointers)
            x = left[0]
            y = right[0]
            ptr[x] = ptr[y]

    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    solve()
