"""Checker for p324: usage checker.py <in> <out> <ans>; exit 0 = accept.

Per document the error/no-error verdict is deterministic and must match
the reference. A formatted line is a certificate validated against the
INPUT document: same word sequence, same dash count in every gap, and
gaps rendered as a single space (0 dashes), a bare hyphen between words
(1 dash), or runs of 2-3 dashes each surrounded by single spaces. Any
valid 2/3 decomposition is accepted, not just the reference's.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def is_letter(c):
    return ("a" <= c <= "z") or ("A" <= c <= "Z")


def parse_doc(line):
    """Return (words, gap dash counts, is_error) for one input document."""
    if len(line) < 2 or line[0] != '"' or line[-1] != '"':
        return [], [], True
    text = line[1:-1]
    words, dashes, dash_count = [], [], 0
    i = 0
    while i < len(text):
        c = text[i]
        if c == " ":
            i += 1
        elif c == "-":
            dash_count += 1
            i += 1
        elif is_letter(c):
            dashes.append(dash_count)
            dash_count = 0
            j = i
            while j < len(text) and is_letter(text[j]):
                j += 1
            words.append(text[i:j])
            i = j
        else:
            return [], [], True
    dashes.append(dash_count)
    if not words:
        return words, dashes, dash_count == 1
    return words, dashes, dashes[0] == 1 or dashes[-1] == 1


def tokenize(text):
    """Split into maximal runs tagged 'w'/'d'/'s'; None on bad char."""
    tokens = []
    i = 0
    while i < len(text):
        c = text[i]
        if is_letter(c):
            kind = "w"
        elif c == "-":
            kind = "d"
        elif c == " ":
            kind = "s"
        else:
            return None
        j = i
        while j < len(text) and (
            (kind == "w" and is_letter(text[j]))
            or (kind == "d" and text[j] == "-")
            or (kind == "s" and text[j] == " ")
        ):
            j += 1
        tokens.append((kind, text[i:j]))
        i = j
    return tokens


def check_gap(tokens, k, lineno, interior):
    """Validate the token run of one gap holding k input dashes."""
    if k == 0:
        want = [("s", " ")] if interior else []
        if tokens != want:
            reject(f"line {lineno}: gap with 0 dashes formatted incorrectly")
        return
    if k == 1:
        if not interior or tokens != [("d", "-")]:
            reject(f"line {lineno}: single dash must be a bare hyphen between words")
        return
    if len(tokens) < 3 or len(tokens) % 2 == 0:
        reject(f"line {lineno}: dash gap must be space-separated runs")
    total = 0
    for idx, (kind, run) in enumerate(tokens):
        if idx % 2 == 0:
            if kind != "s" or run != " ":
                reject(f"line {lineno}: dash run not surrounded by single spaces")
        else:
            if kind != "d" or len(run) not in (2, 3):
                reject(f"line {lineno}: dash run must be 2 or 3 dashes")
            total += len(run)
    if total != k:
        reject(f"line {lineno}: gap has {total} dashes, input has {k}")


def check_line(out_line, words, dashes, lineno):
    out_line = out_line.rstrip()
    if len(out_line) < 2 or out_line[0] != '"' or out_line[-1] != '"':
        reject(f"line {lineno}: formatted text must be enclosed in quotes")
    tokens = tokenize(out_line[1:-1])
    if tokens is None:
        reject(f"line {lineno}: invalid character in output")

    out_words = [run for kind, run in tokens if kind == "w"]
    if out_words != words:
        reject(f"line {lineno}: word sequence differs from input")

    gaps = []
    current = []
    for tok in tokens:
        if tok[0] == "w":
            gaps.append(current)
            current = []
        else:
            current.append(tok)
    gaps.append(current)

    if not words:
        check_gap(gaps[0], dashes[0], lineno, False)
        return
    for g in range(len(dashes)):
        interior = 0 < g < len(dashes) - 1
        check_gap(gaps[g], dashes[g], lineno, interior)


def main():
    in_lines = open(sys.argv[1]).read().split("\n")
    out_lines = open(sys.argv[2]).read().split("\n")
    ans_lines = [l for l in open(sys.argv[3]).read().split("\n") if l.strip()]

    n = int(in_lines[0].split()[0])
    while out_lines and not out_lines[-1].strip():
        out_lines.pop()
    if len(out_lines) != n:
        reject(f"expected {n} output lines, got {len(out_lines)}")
    if len(ans_lines) != n:
        reject(f"internal: reference has {len(ans_lines)} lines for {n} docs")

    for i in range(n):
        words, dashes, is_error = parse_doc(in_lines[1 + i].rstrip("\r"))
        ref_error = ans_lines[i].strip() == "error"
        if is_error != ref_error:
            reject(f"internal: checker/reference error mismatch on line {i + 1}")
        got = out_lines[i].strip()
        if is_error:
            if got != "error":
                reject(f"line {i + 1}: expected 'error', got '{got}'")
        else:
            if got == "error":
                reject(f"line {i + 1}: said 'error' but formatting is possible")
            check_line(out_lines[i], words, dashes, i + 1)

    sys.exit(0)


if __name__ == "__main__":
    main()
