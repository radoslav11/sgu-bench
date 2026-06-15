"""Checker for p121: usage checker.py <in> <out> <ans>; exit 0 = accept.

Solvability is deterministic: if the reference answer is "No solution"
the output must say so too, and vice versa. A claimed painting is a
certificate validated against the input: every island line repeats the
input list's length with colors 1/2 followed by the terminating 0, the
two occurrences of each bridge get the same color, and every island
with two or more bridges sees both colors. Any valid painting is
accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_text = open(sys.argv[2]).read()
    ans_text = open(sys.argv[3]).read()

    n = int(in_tokens[0])
    pos = 1
    lists = []
    for _ in range(n):
        cur = []
        while True:
            v = int(in_tokens[pos])
            pos += 1
            if v == 0:
                break
            cur.append(v)
        lists.append(cur)

    ans_no = "no solution" in ans_text.lower()
    out_no = "no solution" in out_text.lower()

    if ans_no or out_no:
        if ans_no and out_no:
            sys.exit(0)
        if ans_no:
            reject("no painting exists, but output claims one")
        reject("a painting exists, but output says 'No solution'")

    out_tokens = out_text.split()
    expected_len = sum(len(l) + 1 for l in lists)
    if len(out_tokens) != expected_len:
        reject(f"expected {expected_len} tokens, got {len(out_tokens)}")

    color = {}
    seen_colors = [set() for _ in range(n + 1)]
    degree = [0] * (n + 1)
    pos = 0
    for u in range(1, n + 1):
        for v in lists[u - 1]:
            tok = out_tokens[pos]
            pos += 1
            if tok not in ("1", "2"):
                reject(f"island {u}: color must be 1 or 2, got '{tok}'")
            c = int(tok)
            key = (min(u, v), max(u, v))
            if key in color:
                if color[key] != c:
                    reject(f"bridge {key} painted both {color[key]} and {c}")
            else:
                color[key] = c
                degree[u] += 1
                degree[v] += 1
            seen_colors[u].add(c)
            seen_colors[v].add(c)
        if out_tokens[pos] != "0":
            reject(f"island {u}: list not terminated by 0")
        pos += 1

    for u in range(1, n + 1):
        if degree[u] >= 2 and len(seen_colors[u]) != 2:
            reject(f"island {u} has {degree[u]} bridges but only "
                   f"colors {sorted(seen_colors[u])}")

    sys.exit(0)


if __name__ == "__main__":
    main()
