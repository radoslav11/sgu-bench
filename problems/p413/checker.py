"""Checker for p413: usage checker.py <in> <out> <ans>; exit 0 = accept.

Multiple divisions are valid, so the output is judged only against the input.
For every test case the contestant assigns each city a country label; the
checker verifies the labels are 1..k with no gaps, every country has at least
two cities, and the subgraph induced (using the input roads) on each country
is a tree: connected and with exactly size-1 internal roads. The reference's
own labels in <ans> are never required to match.
"""

import sys
from collections import defaultdict


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tok = open(sys.argv[1]).read().split()
    out_tok = open(sys.argv[2]).read().split()

    ip = 0

    def nxt_in():
        nonlocal ip
        v = int(in_tok[ip])
        ip += 1
        return v

    op = 0

    def nxt_out():
        nonlocal op
        if op >= len(out_tok):
            reject("output ended early")
        try:
            v = int(out_tok[op])
        except ValueError:
            reject(f"non-integer token '{out_tok[op]}' in output")
        op += 1
        return v

    tst = nxt_in()
    for tc in range(1, tst + 1):
        n = nxt_in()
        m = nxt_in()
        edges = []
        for _ in range(m):
            a = nxt_in()
            b = nxt_in()
            edges.append((a, b))

        country = [nxt_out() for _ in range(n)]

        # Labels must be a contiguous range 1..k.
        labels = set(country)
        k = max(country)
        if min(country) < 1:
            reject(f"test {tc}: country label below 1")
        if labels != set(range(1, k + 1)):
            reject(f"test {tc}: country labels are not 1..{k} without gaps")

        members = defaultdict(list)
        for v in range(1, n + 1):
            members[country[v - 1]].append(v)

        adj = defaultdict(list)
        internal = defaultdict(int)
        for a, b in edges:
            if country[a - 1] == country[b - 1]:
                c = country[a - 1]
                adj[a].append(b)
                adj[b].append(a)
                internal[c] += 1

        for c in range(1, k + 1):
            mem = members[c]
            if len(mem) < 2:
                reject(f"test {tc}: country {c} has fewer than 2 cities")
            if internal[c] != len(mem) - 1:
                reject(
                    f"test {tc}: country {c} has {internal[c]} internal roads, "
                    f"a tree on {len(mem)} cities needs {len(mem) - 1}"
                )
            seen = {mem[0]}
            stack = [mem[0]]
            while stack:
                x = stack.pop()
                for y in adj[x]:
                    if y not in seen:
                        seen.add(y)
                        stack.append(y)
            if len(seen) != len(mem):
                reject(f"test {tc}: country {c} is not connected")

    if op != len(out_tok):
        reject("extra tokens at end of output")

    sys.exit(0)


if __name__ == "__main__":
    main()
