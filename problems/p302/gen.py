"""Tests for p302 (BHTML 1.0): render <UP>/<DOWN> tag casing.

The string is latin letters plus correctly nested <UP></UP> (9 chars a
pair) and <DOWN></DOWN> (13 chars a pair) tags, total length <= 1000.
A budgeted random walk emits letters / opens / closes while always
keeping enough room to close the open stack, then pads with letters to
hit the exact target length. Seed 2 is maximal nesting depth, seed 3 a
maximal random mix; seed 4 has no tags at all.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


def rand_text(target):
    parts = []
    length = 0
    stack = []  # close-tag lengths of currently open tags
    while length < target:
        rem = target - length
        need = sum(stack)
        choices = []
        if rem > need:
            choices.append("letter")
        if rem >= need + 9:
            choices += ["open"] * 2
        if stack and rem >= need:
            choices.append("close")
        if not choices:
            break
        act = rng.choice(choices)
        if act == "letter":
            parts.append(rng.choice(LETTERS))
            length += 1
        elif act == "open":
            if rem >= need + 13 and rng.random() < 0.5:
                parts.append("<DOWN>")
                stack.append(7)
                length += 6
            else:
                parts.append("<UP>")
                stack.append(5)
                length += 4
        else:
            close = stack.pop()
            parts.append("</UP>" if close == 5 else "</DOWN>")
            length += close
    while stack:
        close = stack.pop()
        parts.append("</UP>" if close == 5 else "</DOWN>")
        length += close
    parts.append("".join(rng.choice(LETTERS) for _ in range(target - length)))
    return "".join(parts)


def deepest(target):
    # Alternate <UP>/<DOWN> nesting as deep as the budget allows, one
    # letter in the innermost body, letters padding the tail.
    parts = []
    closes = []
    length = 0
    i = 0
    while True:
        tag, close = (("<UP>", "</UP>") if i % 2 == 0 else ("<DOWN>", "</DOWN>"))
        if length + len(tag) + len(close) + sum(len(c) for c in closes) + 1 > target:
            break
        parts.append(tag)
        closes.append(close)
        length += len(tag) + len(close)
        i += 1
    parts.append(rng.choice(LETTERS))
    length += 1
    parts.extend(reversed(closes))
    parts.append("".join(rng.choice(LETTERS) for _ in range(target - length)))
    return "".join(parts)


if seed == 1:
    print("a")
elif seed == 2:
    print(deepest(1000))
elif seed == 3:
    print(rand_text(1000))
elif seed == 4:
    print("".join(rng.choice(LETTERS) for _ in range(1000)))  # no tags
elif seed == 5:
    print("<UP></UP>" + "x" * 13 + "<DOWN></DOWN>")  # empty bodies
elif seed == 6:
    print(rand_text(rng.randint(9, 60)))
elif seed == 7:
    print(rand_text(rng.randint(100, 400)))
elif seed == 8:
    print(rand_text(1000))
else:
    print(rand_text(rng.randint(500, 1000)))
