"""Checker for p264: usage checker.py <in> <out> <ans>; exit 0 = accept.

The YES/NO token must match the reference. A YES must be followed by a
perfect assignment (every man and every woman exactly once) that is
checked for stability against the preference lists from the input. Any
stable matching is accepted, not just the reference's.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    pos = 0
    n = int(in_tokens[pos])
    pos += 1

    men = []
    man_id = {}
    men_pref = []
    for i in range(n):
        name = in_tokens[pos]
        pos += 1
        man_id[name] = i
        men.append(name)
        men_pref.append(in_tokens[pos:pos + n])
        pos += n

    women = []
    woman_id = {}
    women_pref = []
    for i in range(n):
        name = in_tokens[pos]
        pos += 1
        woman_id[name] = i
        women.append(name)
        women_pref.append(in_tokens[pos:pos + n])
        pos += n

    expected = ans_tokens[0].upper()
    if not out_tokens:
        reject("empty output")
    got = out_tokens[0].upper()
    if got not in ("YES", "NO"):
        reject(f"first token must be YES/NO, got '{out_tokens[0]}'")
    if got != expected:
        reject(f"answer is '{expected}', got '{got}'")
    if got == "NO":
        if len(out_tokens) > 1:
            reject("trailing output after 'NO'")
        sys.exit(0)

    pairs = out_tokens[1:]
    if len(pairs) != 2 * n:
        reject(f"expected {2 * n} tokens of pairs, got {len(pairs)}")

    man_match = [-1] * n
    woman_match = [-1] * n
    for i in range(n):
        m_name, w_name = pairs[2 * i], pairs[2 * i + 1]
        if m_name not in man_id:
            reject(f"unknown man '{m_name}'")
        if w_name not in woman_id:
            reject(f"unknown woman '{w_name}'")
        m, w = man_id[m_name], woman_id[w_name]
        if man_match[m] != -1:
            reject(f"man '{m_name}' assigned twice")
        if woman_match[w] != -1:
            reject(f"woman '{w_name}' assigned twice")
        man_match[m] = w
        woman_match[w] = m

    # rank[w][m] = position of man m in woman w's list (lower is better)
    woman_rank = [[0] * n for _ in range(n)]
    for w in range(n):
        row = woman_rank[w]
        for r, m_name in enumerate(women_pref[w]):
            row[man_id[m_name]] = r

    for m in range(n):
        pm = man_match[m]
        for w_name in men_pref[m]:
            w = woman_id[w_name]
            if w == pm:
                break
            # m prefers w over his partner; stable only if w prefers hers
            if woman_rank[w][m] < woman_rank[w][woman_match[w]]:
                reject(
                    f"unstable pair: {men[m]} and {women[w]} prefer "
                    f"each other to their partners"
                )

    sys.exit(0)


if __name__ == "__main__":
    main()
