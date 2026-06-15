"""Checker for p195: usage checker.py <in> <out> <ans>; exit 0 = accept.

The total amount is deterministic and must match the reference. The
receiver list is a certificate: receivers must be listed in ascending
order, each receiver's chief must not himself receive, and no chief may
grant more than one subordinate. Any valid optimal set is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_tokens[0])
    parent = [0, 0] + [int(x) for x in in_tokens[1:n]]

    expected_total = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        got_total = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer, got '{out_tokens[0]}'")
    if got_total != expected_total:
        reject(f"total is {expected_total}, got {got_total}")

    receivers = []
    for tok in out_tokens[1:]:
        try:
            receivers.append(int(tok))
        except ValueError:
            reject(f"bad receiver token '{tok}'")
    if len(receivers) * 1000 != expected_total:
        reject(
            f"total {expected_total} implies {expected_total // 1000} "
            f"receivers, listed {len(receivers)}"
        )

    used_chief = set()
    prev = 0
    for r in receivers:
        if r <= prev:
            reject("receivers not in strictly ascending order")
        prev = r
        if r < 2 or r > n:
            reject(f"receiver {r} out of range (node 1 has no chief)")
        if parent[r] in used_chief:
            reject(f"chief {parent[r]} grants more than one subordinate")
        used_chief.add(parent[r])

    recv_set = set(receivers)
    for r in receivers:
        if parent[r] in recv_set:
            reject(f"chief {parent[r]} both receives and grants")

    sys.exit(0)


if __name__ == "__main__":
    main()
