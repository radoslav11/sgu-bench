"""Checker for p393: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal button count K must match the reference's. The partition
itself is a certificate: every letter a..(a+N-1) appears on exactly one
button, buttons are non-empty, and no two vocabulary words map to the
same ordered pair of buttons.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    n, m = int(in_tokens[0]), int(in_tokens[1])
    words = in_tokens[2:2 + m]
    k_ref = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        k = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be K, got '{out_tokens[0]}'")
    if k != k_ref:
        reject(f"minimal K is {k_ref}, got {k}")
    buttons = out_tokens[1:]
    if len(buttons) != k:
        reject(f"declared {k} buttons, listed {len(buttons)}")

    alphabet = set(chr(ord('a') + i) for i in range(n))
    button_of = {}
    for bi, btn in enumerate(buttons):
        if not btn:
            reject("empty button")
        for ch in btn:
            if ch not in alphabet:
                reject(f"letter '{ch}' outside the first {n} letters")
            if ch in button_of:
                reject(f"letter '{ch}' placed on more than one button")
            button_of[ch] = bi
    if len(button_of) != n:
        missing = sorted(alphabet - set(button_of))
        reject(f"letters not placed: {''.join(missing)}")

    seen = {}
    for w in words:
        key = (button_of[w[0]], button_of[w[1]])
        if key in seen:
            reject(f"words '{seen[key]}' and '{w}' share a button sequence")
        seen[key] = w

    sys.exit(0)


if __name__ == "__main__":
    main()
