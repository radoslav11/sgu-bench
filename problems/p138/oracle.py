import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    a = list(map(int, data[1:]))

    total = sum(a)
    assert total % 2 == 0
    G = total // 2

    # Create sorted order of players by descending games
    perm = list(range(n))
    perm.sort(key=lambda i: -a[i])

    # b[i] = remaining games of the player at sorted position i
    b = [a[i] for i in perm]

    matches = []  # will store tuples (winner_idx, loser_idx or -1)
    p = 0         # current champion pointer

    # Phase 1: decide winners & placeholders
    for _ in range(G):
        if b[p] > 1:
            # champion wins, postpone picking loser
            matches.append([p, -1])
            b[p] -= 1
        else:
            # champion must lose to pass on the token
            matches.append([p+1, p])
            b[p]   -= 1
            b[p+1] -= 1
            p += 1  # new champion

    # Phase 2: fill in the losers for placeholder matches
    q = 0
    for mv in matches:
        if mv[1] != -1:
            continue
        # find next player with leftover games
        while b[q] == 0:
            q += 1
        mv[1] = q
        b[q] -= 1

    # Print results, translating back to 1-based original indices
    out = [str(G)]
    for w, l in matches:
        # perm[w] is the original index, add 1 for 1-based
        out.append(f"{perm[w]+1} {perm[l]+1}")
    print("\n".join(out))

if __name__ == "__main__":
    main()
