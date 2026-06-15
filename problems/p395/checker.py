"""Checker for p395: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal polished-log length M must match the reference's. The log
itself is a certificate: it must be plausible (nobody enters twice or
leaves while outside, every "= c" equals the current occupancy), keep
the original records as a subsequence in order, and respect the format
limits (names 1-16 lowercase letters, counts 0..100).
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_records(tokens, count, where):
    records = []
    if len(tokens) != 2 * count:
        reject(f"{where}: expected {count} records, got {len(tokens) // 2}")
    for i in range(count):
        t, v = tokens[2 * i], tokens[2 * i + 1]
        if t == '=':
            if not v.isdigit():
                reject(f"{where}: bad count record '= {v}'")
            val = int(v)
            if not (0 <= val <= 100):
                reject(f"{where}: count {val} out of range 0..100")
            records.append(('=', val))
        elif t in ('+', '-'):
            if not (1 <= len(v) <= 16 and v.isalpha() and v.islower()):
                reject(f"{where}: bad name '{v}'")
            records.append((t, v))
        else:
            reject(f"{where}: unknown record type '{t}'")
    return records


def main():
    in_lines = open(sys.argv[1]).read().split('\n')
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n = int(in_lines[0])
    in_tokens = " ".join(in_lines[1:1 + n]).split()
    original = parse_records(in_tokens, n, "input")

    m_ref = int(ans_tokens[0])
    if not out_tokens:
        reject("empty output")
    try:
        m = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be M, got '{out_tokens[0]}'")
    if m != m_ref:
        reject(f"minimal M is {m_ref}, got {m}")
    log = parse_records(out_tokens[1:], m, "output")

    inside = set()
    ptr = 0
    for idx, (t, v) in enumerate(log):
        if t == '+':
            if v in inside:
                reject(f"record {idx + 1}: '{v}' enters while inside")
            inside.add(v)
        elif t == '-':
            if v not in inside:
                reject(f"record {idx + 1}: '{v}' leaves while outside")
            inside.remove(v)
        else:
            if v != len(inside):
                reject(
                    f"record {idx + 1}: log says {v} guests, "
                    f"actually {len(inside)}"
                )
        if ptr < n and (t, v) == original[ptr]:
            ptr += 1
    if ptr != n:
        reject(f"only {ptr} of {n} original records kept in order")

    sys.exit(0)


if __name__ == "__main__":
    main()
