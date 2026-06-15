"""Checker for p240: usage checker.py <in> <out> <ans>; exit 0 = accept.

YES/NO and the optimal maximal heat are deterministic and must match the
reference. The escape way is any walk: it must start at S, end at an
exit, use existing corridors, and its maximal heat (entering a corridor
at time t costs R + (t + T) * P at its end) must equal the optimum and
not exceed H.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    n, m, h, s, e = (int(next(it)) for _ in range(5))
    edge = {}
    for _ in range(m):
        a, b, t, r, p = (int(next(it)) for _ in range(5))
        edge[(a, b)] = edge[(b, a)] = (t, r, p)
    exits = {int(next(it)) for _ in range(e)}

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        sys.exit(0)

    best = int(ans_tokens[1])
    try:
        value = int(out_tokens[1])
        z = int(out_tokens[2])
        way = [int(x) for x in out_tokens[3 : 3 + z]]
    except (IndexError, ValueError):
        reject("malformed YES output (need heat, length, junctions)")
    if value != best:
        reject(f"optimal heat is {best}, got {value}")
    if len(way) != z or len(out_tokens) > 3 + z:
        reject("way length does not match declared count")
    if z < 1 or any(not (1 <= v <= n) for v in way):
        reject("junction out of range or empty way")
    if way[0] != s:
        reject(f"way must start at {s}, starts at {way[0]}")
    if way[-1] not in exits:
        reject(f"way ends at {way[-1]}, which is not an exit")

    time = 0
    heat = 0
    for u, v in zip(way, way[1:]):
        if (u, v) not in edge:
            reject(f"no corridor between {u} and {v}")
        t, r, p = edge[(u, v)]
        time += t
        heat = max(heat, r + time * p)

    if heat > h:
        reject(f"way reaches heat {heat} > H = {h}")
    if heat != best:
        reject(f"way has maximal heat {heat}, optimum is {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
