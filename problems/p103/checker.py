"""Checker for p103: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimum travel time (or 0 for "no path") is deterministic and must
match the reference. The path is a certificate: it must start at the
source, end at the destination, use existing roads, and a greedy
earliest-departure simulation along it must finish in exactly the
claimed optimal time. Any such path is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def get_color(light, t):
    is_blue0, rem0, t_blue, t_purple = light
    if t < rem0:
        return 1 if is_blue0 else 0
    t -= rem0
    tt = t % (t_blue + t_purple)
    if is_blue0:
        return 0 if tt < t_purple else 1
    return 1 if tt < t_blue else 0


def time_to_next_change(light, t):
    is_blue0, rem0, t_blue, t_purple = light
    if t < rem0:
        return rem0 - t
    t -= rem0
    tt = t % (t_blue + t_purple)
    first = t_purple if is_blue0 else t_blue
    if tt < first:
        return first - tt
    return t_blue + t_purple - tt


def earliest_departure(lu, lv, t):
    for _ in range(1000):
        if get_color(lu, t) == get_color(lv, t):
            return t
        t += min(time_to_next_change(lu, t), time_to_next_change(lv, t))
    return None


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    src, dst = int(next(it)), int(next(it))
    n, m = int(next(it)), int(next(it))
    lights = []
    for _ in range(n):
        c = next(it)
        r, tb, tp = int(next(it)), int(next(it)), int(next(it))
        lights.append((c == "B", r, tb, tp))
    road = {}
    for _ in range(m):
        u, v, w = int(next(it)), int(next(it)), int(next(it))
        road[(u, v)] = w
        road[(v, u)] = w

    if not out_tokens:
        reject("empty output")

    if ans_tokens == ["0"]:
        if out_tokens != ["0"]:
            reject("no path exists, expected the single integer 0")
        sys.exit(0)

    best = int(ans_tokens[0])
    try:
        claimed = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the time, got '{out_tokens[0]}'")
    if claimed != best:
        reject(f"optimal time is {best}, claimed {claimed}")

    path = []
    for tok in out_tokens[1:]:
        try:
            path.append(int(tok))
        except ValueError:
            reject(f"bad junction id '{tok}'")
    if len(path) < 2:
        reject("path must list at least source and destination")
    if path[0] != src:
        reject(f"path starts at {path[0]}, source is {src}")
    if path[-1] != dst:
        reject(f"path ends at {path[-1]}, destination is {dst}")
    if any(not (1 <= x <= n) for x in path):
        reject("junction id out of range")

    t = 0
    for a, b in zip(path, path[1:]):
        if (a, b) not in road:
            reject(f"no road between {a} and {b}")
        depart = earliest_departure(lights[a - 1], lights[b - 1], t)
        if depart is None:
            reject(f"lights at {a} and {b} never match after time {t}")
        t = depart + road[(a, b)]

    if t != best:
        reject(f"path takes {t}, claimed optimum is {best}")

    sys.exit(0)


if __name__ == "__main__":
    main()
