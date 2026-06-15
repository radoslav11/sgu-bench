import sys


def solve():
    # Read all integers from standard input.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # If input is empty, do nothing.
    if not data:
        return

    # Pointer into the flat input array.
    ptr = 0

    # Number of colors.
    c = data[ptr]
    ptr += 1

    # Big corner has size 2^k.
    k = data[ptr]
    ptr += 1

    # a[i][j] = number of available corners of color i and size 2^j.
    a = []

    # Read the c by k table.
    for _ in range(c):
        row = data[ptr:ptr + k]
        ptr += k
        a.append(row)

    # Compute normalized volume of each color.
    #
    # A corner of size 2^j has normalized volume 8^j.
    # Python integers are arbitrary precision, so we can store huge values directly.
    volumes = []

    for i in range(c):
        total = 0

        for j in range(k):
            cnt = a[i][j]

            # cnt * 8^j.
            #
            # Since 8^j == 2^(3j), shifting left by 3*j is equivalent.
            total += cnt << (3 * j)

        volumes.append(total)

    # Sort color indices by decreasing total volume.
    order = sorted(range(c), key=lambda i: volumes[i], reverse=True)

    # sum_caps[j] = total available pieces of size 2^j among chosen colors.
    sum_caps = [0] * k

    # x[j] = number of pieces of size 2^j that we will actually use.
    x = [0] * k

    # Chosen color indices.
    chosen = []

    def try_greedy():
        """
        Tests whether currently chosen colors can build the big corner.

        Also fills array x, where x[j] is the number of pieces of size 2^j
        that should be used.

        We process sizes from large to small.

        To build a size-2^k corner, we initially need 8 corners of size 2^(k-1).
        If some of those are missing, each missing one is replaced by 8 smaller
        corners.
        """

        # We want to modify outer variable x.
        nonlocal x

        # Reset chosen counts.
        x = [0] * k

        # Initial demand at size k - 1.
        need = 8

        # Process sizes from k - 1 down to 0.
        for j in range(k - 1, -1, -1):
            # If enough pieces of this size exist, use exactly need and finish.
            if sum_caps[j] >= need:
                x[j] = need
                return True

            # Otherwise, use all available pieces of this size.
            x[j] = sum_caps[j]

            # Missing pieces of size j.
            missing = need - sum_caps[j]

            # Each missing size-j piece requires 8 size-(j-1) pieces.
            need = missing * 8

        # If demand remains after size 0, construction is impossible.
        return False

    # Whether a feasible prefix of colors has been found.
    feasible = False

    # Add colors from largest volume to smallest.
    for color in order:
        # Add this color to the chosen set.
        chosen.append(color)

        # Add this color's pieces to total capacities.
        for j in range(k):
            sum_caps[j] += a[color][j]

        # Check if the current chosen colors are sufficient.
        if try_greedy():
            feasible = True
            break

    # If even all colors are insufficient, output NO SOLUTION.
    if not feasible:
        sys.stdout.write("NO SOLUTION\n")
        return

    # Store output triples:
    #   color, size, count
    result = []

    # For every size, distribute the required count among chosen colors.
    for j in range(k):
        # Number of size-2^j pieces still needed.
        need = x[j]

        # Take pieces from chosen colors.
        for color in chosen:
            # If enough pieces of this size have been taken, stop.
            if need == 0:
                break

            # Number of pieces we can take from this color and size.
            take = min(a[color][j], need)

            # If positive, record this type.
            if take > 0:
                # Output colors are 1-indexed.
                result.append((color + 1, j, take))

                # Decrease remaining demand.
                need -= take

    # Prepare output lines.
    out = []

    # First line: number of used types.
    out.append(str(len(result)))

    # Then each used type.
    for color, size, count in result:
        out.append(f"{color} {size} {count}")

    # Print answer.
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
