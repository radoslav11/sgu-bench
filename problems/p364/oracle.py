import sys


def solve():
    # Read all integers from input.
    data = list(map(int, sys.stdin.read().split()))

    # Pointer into the input array.
    idx = 0

    # Number of lemmings and interval between appearances.
    n = data[idx]
    s = data[idx + 1]
    idx += 2

    # Start point A = (x, h), home point = (a, b).
    x = data[idx]
    h = data[idx + 1]
    a = data[idx + 2]
    b = data[idx + 3]
    idx += 4

    # Number of platforms.
    m = data[idx]
    idx += 1

    # Platform arrays.
    pl = [0] * m  # left endpoints
    pr = [0] * m  # right endpoints
    ph = [0] * m  # heights

    # Read platform descriptions.
    for i in range(m):
        pl[i] = data[idx]
        pr[i] = data[idx + 1]
        ph[i] = data[idx + 2]
        idx += 3

    # Finds the highest platform strictly below height from_h
    # whose interior contains x-coordinate fx.
    def next_below(fx, from_h):
        best = -1

        for i in range(m):
            # Must be below current height and contain fx strictly inside.
            if ph[i] < from_h and pl[i] < fx < pr[i]:
                # Pick the highest such platform.
                if best == -1 or ph[i] > ph[best]:
                    best = i

        return best

    # Find the platform containing the home point.
    home_plat = -1
    for i in range(m):
        if ph[i] == b and pl[i] <= a <= pr[i]:
            home_plat = i

    # Process platforms from highest to lowest.
    # Since every fall strictly decreases height, this is topological order.
    order = sorted(range(m), key=lambda i: -ph[i])

    # arrivals[p] is a dictionary for platform p.
    #
    # Key:
    #   (entry_x, direction)
    #
    # direction:
    #   1  means moving right
    #  -1  means moving left
    #
    # Value:
    #   (turns, distance)
    #
    # Costs are compared lexicographically.
    arrivals = [dict() for _ in range(m)]

    # Find first platform reached from the starting point.
    start = next_below(x, h)

    # Infinity for unreachable answer.
    INF = 10**18

    # Best answer so far: minimum turns, then minimum distance.
    best_turns = INF
    best_dist = INF

    # Initialize starting state if the lemming lands on a platform.
    if start != -1:
        # Start moving right after falling vertically.
        # Distance is vertical fall from h to platform height.
        arrivals[start][(x, 1)] = (0, h - ph[start])

    # Dynamic programming over platforms from top to bottom.
    for plat in order:
        # Make a list of items to safely iterate over the current states.
        # Transitions always go to lower platforms, so this dict is not modified
        # by transitions back into itself, but list() is harmless and safe.
        for (ex, direction), (turns, dist) in list(arrivals[plat].items()):

            # If current platform is the home platform, try finishing here.
            if plat == home_plat:
                diff = a - ex

                # If the home is ahead or exactly at the current position,
                # no extra reflector is needed. Otherwise, turn at ex.
                extra_turn = 0 if diff * direction >= 0 else 1

                reach_turns = turns + extra_turn
                reach_dist = dist + abs(diff)

                # Keep lexicographically best route to home.
                if (reach_turns, reach_dist) < (best_turns, best_dist):
                    best_turns = reach_turns
                    best_dist = reach_dist

            # The end reached by continuing in the current direction.
            if direction > 0:
                forward_end = pr[plat]
                back_end = pl[plat]
            else:
                forward_end = pl[plat]
                back_end = pr[plat]

            # Two possible choices from this platform:
            #
            # 1. Continue forward to the forward end.
            # 2. Turn immediately at entry and go to the opposite end.
            moves = [
                (forward_end, direction, turns),
                (back_end, -direction, turns + 1),
            ]

            for end_x, new_direction, new_turns in moves:
                # Find next platform after falling from end_x.
                nb = next_below(end_x, ph[plat])

                # If no platform is below, this path falls forever.
                if nb == -1:
                    continue

                # Horizontal walking on current platform plus vertical fall.
                new_dist = dist + abs(end_x - ex) + (ph[plat] - ph[nb])

                # New state on the lower platform.
                state = (end_x, new_direction)

                # Relax if this is the first or better way to reach state.
                old = arrivals[nb].get(state)

                if old is None or (new_turns, new_dist) < old:
                    arrivals[nb][state] = (new_turns, new_dist)

    # If no route exists, or if every lemming must be stopped, nobody reaches home.
    if best_turns == INF or best_turns >= n:
        print("0 0")
        return

    # Number of successful lemmings.
    successful = n - best_turns

    # Last lemming appears at time (n - 1) * s.
    # Then it needs best_dist seconds to reach home.
    last_time = (n - 1) * s + best_dist

    print(successful, last_time)


if __name__ == "__main__":
    solve()
