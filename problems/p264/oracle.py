import sys
from collections import deque

def solve() -> None:
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    n = int(next(it))  # number of men and women

    # Read men block: each line is
    # <man> <woman1> <woman2> ... <womanN>
    men_names = [None] * n
    men_pref_names = [[None] * n for _ in range(n)]
    man_id = {}

    for i in range(n):
        name = next(it).decode()
        men_names[i] = name
        man_id[name] = i
        for j in range(n):
            men_pref_names[i][j] = next(it).decode()

    # Read women block: each line is
    # <woman> <man1> <man2> ... <manN>
    women_names = [None] * n
    women_pref_names = [[None] * n for _ in range(n)]
    woman_id = {}

    for i in range(n):
        name = next(it).decode()
        women_names[i] = name
        woman_id[name] = i
        for j in range(n):
            women_pref_names[i][j] = next(it).decode()

    # Convert men's preference lists from names -> woman ids
    men_pref = [[0] * n for _ in range(n)]
    for m in range(n):
        for k in range(n):
            men_pref[m][k] = woman_id[men_pref_names[m][k]]

    # Convert women's preference lists from names -> man ids
    women_pref = [[0] * n for _ in range(n)]
    for w in range(n):
        for k in range(n):
            women_pref[w][k] = man_id[women_pref_names[w][k]]

    # Build rank table for women:
    # woman_rank[w][m] = how much woman w likes man m (smaller is better)
    woman_rank = [[0] * n for _ in range(n)]
    for w in range(n):
        for pos in range(n):
            m = women_pref[w][pos]
            woman_rank[w][m] = pos

    # Matching arrays: -1 means free
    man_match = [-1] * n
    woman_match = [-1] * n

    # Next proposal index for each man
    man_next = [0] * n

    # Queue of free men
    q = deque(range(n))

    # Gale–Shapley loop
    while q:
        m = q.popleft()

        # Propose to the next woman on m's list
        w = men_pref[m][man_next[m]]
        man_next[m] += 1

        if woman_match[w] == -1:
            # Woman is free: match
            woman_match[w] = m
            man_match[m] = w
        else:
            # Woman is taken: compare current fiancé and new proposer
            curr = woman_match[w]
            if woman_rank[w][m] < woman_rank[w][curr]:
                # Woman prefers m, so she switches
                woman_match[w] = m
                man_match[m] = w

                # Old fiancé becomes free again
                man_match[curr] = -1
                q.append(curr)
            else:
                # Woman rejects m; m stays free and tries again later
                q.append(m)

    # Output (stable matching always exists here)
    out_lines = ["YES"]
    for m in range(n):
        out_lines.append(f"{men_names[m]} {women_names[man_match[m]]}")
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
