import sys


def clockwise_distance(frm, to, n):
    """
    Clockwise distance from position frm to position to
    on a cycle of length n + 1.
    """
    return (to - frm + n + 1) % (n + 1)


def compute_cost(a, b, n):
    """
    Compute the minimum cost to transform cyclic arrangement a into b.

    a and b are lists of length n + 1.
    Position 0 is the virtual zero.
    Positions 1..n are letters encoded as integers 1..n.

    Returns:
        total_cost, moves
    where moves is the sequence of clockwise distances.
    """

    # Recorded move lengths.
    moves = []

    # Total cost.
    total = 0

    # pos_in_b[value] = position of value in target arrangement b.
    pos_in_b = [0] * (n + 1)

    for i in range(n + 1):
        pos_in_b[b[i]] = i

    # perm[i] = where the value currently at position i must go.
    perm = [0] * (n + 1)

    for i in range(n + 1):
        perm[i] = pos_in_b[a[i]]

    # Helper to add one move.
    def push_step(frm, to):
        nonlocal total

        # Compute clockwise distance.
        d = clockwise_distance(frm, to, n)

        # Store this move for reconstruction.
        moves.append(d)

        # Add to total cost.
        total += d

    # Used for cycle decomposition.
    visited = [False] * (n + 1)

    # Process all cycles of perm.
    for i in range(n + 1):
        if visited[i]:
            continue

        visited[i] = True

        # Fixed point, no operation needed.
        if perm[i] == i:
            continue

        # Build current cycle.
        cycle = [i]
        x = perm[i]

        while x != i:
            cycle.append(x)
            visited[x] = True
            x = perm[x]

        cnt = len(cycle)

        # Cycle containing zero.
        if i == 0:
            # Walk through the cycle in reverse order, keeping zero first.
            cycle = [cycle[0]] + list(reversed(cycle[1:]))

            # Add consecutive moves.
            for j in range(1, cnt):
                push_step(cycle[j - 1], cycle[j])

        else:
            # Non-zero cycle.
            # After the zero-cycle is fixed, zero sits at perm[0].
            z = perm[0]

            # Choose a good entry point into this cycle.
            start = 0

            for j in range(cnt):
                x = cycle[j]
                y = cycle[(j + 1) % cnt]

                # If x < y, arc x -> y does not wrap.
                # We need z outside the open arc.
                zero_outside = x < y and (z < x or z > y)

                # If x > y, arc x -> y wraps around.
                # Then z is outside iff y < z < x.
                zero_inside_wrap = y < z < x

                if zero_outside or zero_inside_wrap:
                    start = j

            # Rotate cycle so chosen start is first.
            cycle = cycle[start:] + cycle[:start]

            # Walk the rest in reverse order.
            cycle = [cycle[0]] + list(reversed(cycle[1:]))

            # Enter the cycle from z.
            push_step(z, cycle[0])

            # Traverse the cycle.
            for j in range(1, cnt):
                push_step(cycle[j - 1], cycle[j])

            # Return to z.
            push_step(cycle[-1], z)

    return total, moves


def apply_move(a, p, n):
    """
    Apply one real move of length p to current stable window a[1..n].

    This corresponds to appending:
        a[p], a[1], ..., a[p-1]

    The new stable window becomes:
        a[p+1], ..., a[n], a[p], a[1], ..., a[p-1]
    """

    # Temporary array.
    nxt = [0] * (n + 1)

    # Elements after p move to the front.
    for i in range(p + 1, n + 1):
        nxt[i - p] = a[i]

    # Element p follows.
    nxt[n - p + 1] = a[p]

    # Elements before p follow.
    for i in range(1, p):
        nxt[n - p + 1 + i] = a[i]

    # Keep virtual zero at position 0.
    nxt[0] = 0

    return nxt


def solve():
    data = sys.stdin.read().strip().split()

    n = int(data[0])
    s1 = data[1]
    s2 = data[2]

    # Encode S1 and S2.
    # 0 is the virtual separator.
    a = [0] + [ord(ch) - ord('A') + 1 for ch in s1]
    b = [0] + [ord(ch) - ord('A') + 1 for ch in s2]

    best_cost = 10**9
    best_dir = 0
    best_rot = 0

    # Try both directions:
    # dir = 0: start with S1, reach S2.
    # dir = 1: start with S2, reach S1.
    for direction in range(2):
        # Try all cyclic rotations of target.
        for r in range(n + 1):
            cost, _ = compute_cost(a, b, n)

            if cost < best_cost:
                best_cost = cost
                best_dir = direction
                best_rot = r

            # Rotate b left by one.
            b = b[1:] + b[:1]

        # Swap start and target.
        a, b = b, a

    # After two swaps, a and b are back to original.
    # If best direction starts from S2, swap once more.
    if best_dir == 1:
        a, b = b, a

    # Apply best target rotation.
    b = b[best_rot:] + b[:best_rot]

    # Recompute moves for the chosen optimal configuration.
    _, moves = compute_cost(a, b, n)

    # Build output starting from the chosen initial permutation.
    result = []

    for i in range(1, n + 1):
        result.append(chr(ord('A') + a[i] - 1))

    # Replay all moves.
    for p in moves:
        # No characters appended for zero-distance moves.
        if p == 0:
            continue

        # Append a[p].
        result.append(chr(ord('A') + a[p] - 1))

        # Append a[1], ..., a[p-1].
        for i in range(1, p):
            result.append(chr(ord('A') + a[i] - 1))

        # Update current stable window.
        a = apply_move(a, p, n)

    print(''.join(result))


if __name__ == "__main__":
    solve()
