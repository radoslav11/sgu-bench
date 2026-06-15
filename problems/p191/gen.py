"""Tests for p191 (Exhibition): first line "A" or "B", second line a row
of up to 30000 stands over {A, B}.

Strings producible by the process always have odd length, so the maximal
positive tests use 29999 stands. Seed 1 is minimal (one stand). Seed 2 is
a maximal random valid derivation, seed 3 a maximal uniformly random
string (length 30000), seed 4 a maximal expand-first (deepest stack)
derivation, seed 5 a maximal valid string with the last character
flipped. Seeds 6+ are random mid-size, mixing valid and corrupted rows.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def derive(init, length, expand_first=False):
    """Random derivation of the grammar S_X -> opp(X) | X S_opp(X) S_X
    producing exactly `length` characters (length must be odd)."""
    assert length % 2 == 1
    expands_left = (length - 1) // 2
    stack = [init]
    out = []
    remaining = length
    while stack:
        top = stack.pop()
        if expands_left > 0 and (
            expand_first or rng.random() < 2.0 * expands_left / remaining
        ):
            out.append(top)
            stack.append(top)
            stack.append("B" if top == "A" else "A")
            expands_left -= 1
        else:
            out.append("B" if top == "A" else "A")
        remaining -= 1
    return "".join(out)


if seed == 1:
    print("A")
    print("B")
elif seed == 2:
    print("A")
    print(derive("A", 29999))
elif seed == 3:
    print("B")
    print("".join(rng.choice("AB") for _ in range(30000)))
elif seed == 4:
    print("B")
    print(derive("B", 29999, expand_first=True))
elif seed == 5:
    s = derive("A", 29999)
    print("A")
    print(s[:-1] + ("B" if s[-1] == "A" else "A"))
else:
    init = rng.choice("AB")
    length = rng.randint(1, 9999) * 2 + 1
    s = derive(init, length)
    mode = rng.random()
    if mode < 0.35:
        pass  # valid string
    elif mode < 0.6:
        i = rng.randrange(length)
        s = s[:i] + ("B" if s[i] == "A" else "A") + s[i + 1:]
    elif mode < 0.8:
        s = "".join(rng.choice("AB") for _ in range(rng.randint(1, length)))
    else:
        init = "B" if init == "A" else "A"  # wrong starting company
    print(init)
    print(s)
