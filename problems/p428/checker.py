"""Checker for p428: usage checker.py <in> <out> <ans>; exit 0 = accept.

The input is empty and any valid output is accepted (the reference answer
is never compared against). The output must contain exactly 1000 lines,
each "L+R=S" of capital letters with |L| = |R| = |S|, at most 100 chars.
The 1000 rebuses must be pairwise non-isomorphic (canonical first
occurrence relabeling), and each must be proper: exactly one injective
digit assignment, with no multi-digit number starting with 0, satisfying
L + R = S. Properness is verified by a column-wise backtracking counter
that stops as soon as two solutions are found.
"""

import re
import sys

LINE_RE = re.compile(r"^([A-Z]+)\+([A-Z]+)=([A-Z]+)$")


def reject(message):
    print(message)
    sys.exit(1)


def canonical(line):
    mapping = {}
    out = []
    for ch in line:
        if ch in "+=":
            out.append(ch)
        else:
            if ch not in mapping:
                mapping[ch] = chr(ord("A") + len(mapping))
            out.append(mapping[ch])
    return "".join(out)


def count_solutions(L, R, S, limit=2):
    n = len(L)
    letters = sorted(set(L + R + S))
    if len(letters) > 10:
        return 0
    idx = {ch: i for i, ch in enumerate(letters)}
    a = [idx[ch] for ch in reversed(L)]
    b = [idx[ch] for ch in reversed(R)]
    c = [idx[ch] for ch in reversed(S)]
    # letters that may not be 0 (leading letters of multi-digit numbers)
    no_zero = [False] * len(letters)
    if n > 1:
        for ch in (L[0], R[0], S[0]):
            no_zero[idx[ch]] = True

    assign = [-1] * len(letters)
    used = [False] * 10
    count = 0

    def dfs(col, carry):
        nonlocal count
        if col == n:
            if carry == 0:
                count += 1
            return
        ia, ib, ic = a[col], b[col], c[col]
        da = assign[ia]
        for xa in range(10) if da < 0 else (da,):
            if da < 0:
                if used[xa] or (xa == 0 and no_zero[ia]):
                    continue
                assign[ia] = xa
                used[xa] = True
            db = assign[ib]
            for xb in range(10) if db < 0 else (db,):
                if db < 0:
                    if used[xb] or (xb == 0 and no_zero[ib]):
                        continue
                    assign[ib] = xb
                    used[xb] = True
                total = xa + xb + carry
                xc = total % 10
                dc = assign[ic]
                ok = False
                if dc == xc:
                    ok = True
                elif dc < 0 and not used[xc] and not (xc == 0 and no_zero[ic]):
                    assign[ic] = xc
                    used[xc] = True
                    ok = True
                    dc = -1
                else:
                    dc = 0  # marker: nothing to undo
                if ok:
                    dfs(col + 1, total // 10)
                    if dc == -1:
                        assign[ic] = -1
                        used[xc] = False
                if db < 0:
                    assign[ib] = -1
                    used[xb] = False
                if count >= limit:
                    break
            if da < 0:
                assign[ia] = -1
                used[xa] = False
            if count >= limit:
                break

    dfs(0, 0)
    return count


def main():
    out_lines = [ln.strip() for ln in open(sys.argv[2]).read().splitlines()]
    out_lines = [ln for ln in out_lines if ln]

    if len(out_lines) != 1000:
        reject(f"expected exactly 1000 rebuses, got {len(out_lines)}")

    seen = set()
    for i, line in enumerate(out_lines, 1):
        if len(line) > 100:
            reject(f"line {i}: longer than 100 characters")
        m = LINE_RE.match(line)
        if not m:
            reject(f"line {i}: not of the form L+R=S with capital letters")
        L, R, S = m.groups()
        if not (len(L) == len(R) == len(S)):
            reject(f"line {i}: rebus is not aligned")
        canon = canonical(line)
        if canon in seen:
            reject(f"line {i}: isomorphic to an earlier rebus")
        seen.add(canon)
        cnt = count_solutions(L, R, S)
        if cnt == 0:
            reject(f"line {i}: rebus '{line}' has no solution")
        if cnt > 1:
            reject(f"line {i}: rebus '{line}' has more than one solution")

    sys.exit(0)


if __name__ == "__main__":
    main()
