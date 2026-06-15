"""Checker for p477: usage checker.py <in> <out> <ans>; exit 0 = accept.

Existence of a solution is deterministic, so "No solution" must match the
reference. Otherwise the output is any pair (t1, t2) in [1, 10^9] such
that no interval where both doors are open is longer than d, and the
total number of door openings equals the reference's (minimal) count.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def build_events(times, t):
    events = []
    i = 0
    n = len(times)
    while i < n:
        start = times[i] - t
        end = times[i] + t
        i += 1
        while i < n and times[i] - t <= end:
            end = times[i] + t
            i += 1
        events.append((start, 1))
        events.append((end, -1))
    return events


def evaluate(p, q, d, t1, t2):
    """Return (is_feasible, total_openings) for door parameters t1, t2."""
    pe = build_events(p, t1)
    qe = build_events(q, t2)
    openings = len(pe) // 2 + len(qe) // 2

    i = j = 0
    balance = 0
    last_pos = None
    feasible = True
    while i < len(pe) or j < len(qe):
        if j >= len(qe) or (i < len(pe) and pe[i] < qe[j]):
            pos, delta = pe[i]
            i += 1
        else:
            pos, delta = qe[j]
            j += 1
        balance += delta
        if balance == 2:
            last_pos = pos
        else:
            if balance == 1 and last_pos is not None and pos - last_pos > d:
                feasible = False
                break
            last_pos = None
    return feasible, openings


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m, d = int(in_tokens[0]), int(in_tokens[1]), int(in_tokens[2])
    p = [int(x) for x in in_tokens[3:3 + n]]
    q = [int(x) for x in in_tokens[3 + n:3 + n + m]]

    no_solution = ans_tokens[0].lower() == "no"
    out_tokens = out_text.split()
    if not out_tokens:
        reject("empty output")

    if out_tokens[0].lower() == "no":
        if " ".join(out_tokens).lower() != "no solution":
            reject(f"malformed output: '{out_text.strip()[:50]}'")
        if not no_solution:
            reject("said 'No solution' but a solution exists")
        sys.exit(0)

    if no_solution:
        reject("there is no solution, but output gives parameters")

    if len(out_tokens) != 2:
        reject(f"expected two integers, got {len(out_tokens)} tokens")
    try:
        t1, t2 = int(out_tokens[0]), int(out_tokens[1])
    except ValueError:
        reject(f"non-integer parameters: '{out_text.strip()[:50]}'")
    if not (1 <= t1 <= 10**9) or not (1 <= t2 <= 10**9):
        reject(f"parameters out of [1, 10^9]: {t1} {t2}")

    feasible, openings = evaluate(p, q, d, t1, t2)
    if not feasible:
        reject(f"doors with t1={t1}, t2={t2} are both open longer than d")

    rt1, rt2 = int(ans_tokens[0]), int(ans_tokens[1])
    _, ref_openings = evaluate(p, q, d, rt1, rt2)
    if openings != ref_openings:
        reject(f"{openings} openings, optimum is {ref_openings}")

    sys.exit(0)


if __name__ == "__main__":
    main()
