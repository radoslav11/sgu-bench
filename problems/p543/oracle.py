import sys

def can_seat(T, n, r, total_people, min_threes, max_threes):
    """
    Return True if we can seat everyone using exactly T tables.
    We try all feasible counts of 3-sized subgroups (triples).
    """
    empty = T * r - total_people
    if empty < 0:
        return False

    # Try triple counts with correct parity: min_threes, min_threes+2, ...
    threes = min_threes
    while threes <= max_threes:
        remaining = total_people - 3 * threes

        # Remaining people must be representable by pairs => even and non-negative
        if remaining >= 0 and remaining % 2 == 0:
            cur_threes = threes
            cur_r = r

            # If r is odd, handle parity constraints per table
            if cur_r % 2 == 1:
                if cur_threes < T:
                    # Tables without a triple need 1 empty seat each
                    if empty >= (T - cur_threes):
                        return True
                    threes += 2
                    continue
                # Put one triple on every table so leftover capacity per table becomes even
                cur_threes -= T
                cur_r -= 3  # now even

            # Now cur_r is even
            if empty == 0 and (cur_threes % 2 == 1):
                threes += 2
                continue

            # Each 6 seats can contain two triples (3+3), absorbing 2 triples per 6-block
            absorbed = 2 * (cur_r // 6) * T
            if cur_threes <= absorbed:
                return True

            cur_threes -= absorbed

            # Each remaining triple requires one empty seat to "fix" parity/waste a seat
            if empty >= cur_threes:
                return True

        threes += 2

    return False


def solve():
    data = list(map(int, sys.stdin.read().strip().split()))
    n, r = data[0], data[1]
    a = data[2:2+n]

    total_people = sum(a)

    # Compute min/max possible number of triples overall
    min_threes = 0
    max_threes = 0
    for sz in a:
        if sz % 2 == 0:
            max_threes += 2 * (sz // 6)
        else:
            min_threes += 1
            max_threes += 1 + 2 * ((sz - 3) // 6)

    # Binary search for minimal feasible number of tables
    low = (total_people + r - 1) // r
    high = total_people
    ans = high

    while low <= high:
        mid = (low + high) // 2
        if can_seat(mid, n, r, total_people, min_threes, max_threes):
            ans = mid
            high = mid - 1
        else:
            low = mid + 1

    print(ans)


if __name__ == "__main__":
    solve()
