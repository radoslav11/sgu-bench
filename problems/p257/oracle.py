import sys
from itertools import permutations

def solve() -> None:
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)

    # Debts
    P = int(next(it))
    O = int(next(it))
    S = int(next(it))
    debts = [P, O, S]  # person index: 0=P, 1=O, 2=S

    n = int(next(it))

    # Encode each crystal as a 3-bit mask t in [0..7]:
    # bit j = 1 if creditor j marks it as Big (value 2), else Small (value 1)
    crystal_type = []
    counts = [0] * 8
    for _ in range(n):
        s = next(it)           # like "BBS"
        t = 0
        if s[0] == 'B': t |= 1 << 0
        if s[1] == 'B': t |= 1 << 1
        if s[2] == 'B': t |= 1 << 2
        crystal_type.append(t)
        counts[t] += 1

    names = ['P', 'O', 'S']

    # Try all orders of satisfying people (3!) and of spending crystal types (8!)
    for people_perm in permutations([0, 1, 2]):
        for type_perm in permutations(range(8)):
            rem = counts[:]  # remaining crystals per type

            # assign[person][type] = how many crystals of this type are given to that person
            assign = [[0] * 8 for _ in range(3)]

            ok = True
            for person in people_perm:
                debt = debts[person]

                # Phase 1: take as many as possible without overshooting (debt/value)
                for t in type_perm:
                    if debt <= 0:
                        break
                    if rem[t] == 0:
                        continue

                    # Value is 2 if this person's bit is set; otherwise 1
                    value = 2 if ((t >> person) & 1) else 1

                    take = min(rem[t], debt // value)
                    if take:
                        assign[person][t] += take
                        rem[t] -= take
                        debt -= take * value

                # Phase 2: if still not covered, take single crystals (overpay allowed)
                if debt > 0:
                    for t in type_perm:
                        if debt <= 0:
                            break
                        if rem[t] == 0:
                            continue
                        value = 2 if ((t >> person) & 1) else 1
                        assign[person][t] += 1
                        rem[t] -= 1
                        debt -= value

                if debt > 0:
                    ok = False
                    break

            if not ok:
                continue

            # Reconstruct an assignment for each crystal in input order
            out = []
            for t in crystal_type:
                # Give this crystal to any person who still has quota for this type.
                # (Order P,O,S is fine; quotas guarantee feasibility.)
                if assign[0][t] > 0:
                    assign[0][t] -= 1
                    out.append('P')
                elif assign[1][t] > 0:
                    assign[1][t] -= 1
                    out.append('O')
                else:
                    # Must go to S if solution is consistent
                    assign[2][t] -= 1
                    out.append('S')

            sys.stdout.write("".join(out) + "\n")
            return

    sys.stdout.write("no solution\n")


if __name__ == "__main__":
    solve()
