"""Checker for p410: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal day count K is deterministic and must match the reference.
When K <= 1000 a schedule follows: K lines, each "flying mission" or
"science mission to the planet j". Any schedule is accepted if it is
simulated valid: science doubles planet j, a flight needs every planet
>= 1 and subtracts 1 from all, and all planets end at exactly 0.
"""

import re
import sys

SCIENCE_RE = re.compile(r"^science mission to the planet (\d+)$")


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    n = int(in_tokens[0])
    a = [int(x) for x in in_tokens[1 : 1 + n]]

    out_lines = [
        line.strip() for line in open(sys.argv[2]).read().splitlines()
    ]
    out_lines = [line for line in out_lines if line]
    ans_k = int(open(sys.argv[3]).read().split()[0])

    if not out_lines:
        reject("empty output")
    try:
        k = int(out_lines[0])
    except ValueError:
        reject(f"first line must be the day count, got '{out_lines[0]}'")
    if k != ans_k:
        reject(f"minimal number of days is {ans_k}, got {k}")

    if k > 1000:
        if len(out_lines) > 1:
            reject("schedule must be omitted when K > 1000")
        sys.exit(0)

    days = out_lines[1:]
    if len(days) != k:
        reject(f"expected {k} day lines, got {len(days)}")

    # science[j] = list of flight counts observed before each cloning of
    # planet j; flights_total = number of flying-mission days.
    science = {}
    flights_total = 0
    for idx, line in enumerate(days):
        if line == "flying mission":
            flights_total += 1
            continue
        m = SCIENCE_RE.match(line)
        if not m:
            reject(f"day {idx + 1}: unrecognized line '{line}'")
        j = int(m.group(1))
        if not (1 <= j <= n):
            reject(f"day {idx + 1}: planet {j} out of range")
        science.setdefault(j, []).append(flights_total)

    for j in range(1, n + 1):
        v = a[j - 1]
        prev_f = 0
        for f in science.get(j, []):
            v -= f - prev_f
            if v < 0:
                reject(f"planet {j} runs out before its cloning")
            v *= 2
            prev_f = f
        v -= flights_total - prev_f
        if v != 0:
            reject(
                f"planet {j} ends at {v}, not 0 "
                "(or runs out during the flights)"
            )

    sys.exit(0)


if __name__ == "__main__":
    main()
