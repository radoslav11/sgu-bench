import sys


def solve(s):
    if s == "0":
        return "0"

    cnt = [0] * 10
    for ch in s:
        cnt[int(ch)] += 1
    start = tuple(cnt)
    total = len(s)

    # State is the multiset of REMAINING digits (as a 10-tuple of counts).
    # reached[state] = set of remainders mod 17 of the prefix formed so far.
    # parent[(state, rem)] = (prev_state, prev_rem, digit) for reconstruction.
    # Only the multiset matters, so the number of states is the product of
    # (cnt_d + 1), which stays small for 18 digits.
    reached = {}
    parent = {}

    # place the first digit (must be non-zero)
    for d in range(1, 10):
        if start[d] > 0:
            ns = list(start)
            ns[d] -= 1
            ns = tuple(ns)
            rem = d % 17
            reached.setdefault(ns, set())
            if rem not in reached[ns]:
                reached[ns].add(rem)
                parent[(ns, rem)] = (start, None, d)

    # process states from most remaining digits to fewest; placing a digit
    # always drops the remaining count by one, so a single descending sweep
    # over remaining-count levels covers every reachable state exactly once.
    levels = {}
    for st in list(reached.keys()):
        levels.setdefault(sum(st), set()).add(st)

    for rcount in range(total - 1, 0, -1):
        if rcount not in levels:
            continue
        for st in levels[rcount]:
            rems = reached.get(st)
            if not rems:
                continue
            for d in range(10):
                if st[d] == 0:
                    continue
                ns = list(st)
                ns[d] -= 1
                ns = tuple(ns)
                tgt = reached.setdefault(ns, set())
                for rem in rems:
                    nrem = (rem * 10 + d) % 17
                    if nrem not in tgt:
                        tgt.add(nrem)
                        parent[(ns, nrem)] = (st, rem, d)
                levels.setdefault(rcount - 1, set()).add(ns)

    final = tuple([0] * 10)
    if final not in reached or 0 not in reached[final]:
        return "-1"

    digits = []
    state, rem = final, 0
    while True:
        pstate, prem, d = parent[(state, rem)]
        digits.append(d)
        if prem is None:
            break
        state, rem = pstate, prem
    digits.reverse()
    return "".join(map(str, digits))


def main():
    s = sys.stdin.read().strip()
    print(solve(s))


if __name__ == "__main__":
    main()
