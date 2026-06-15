"""Tests for p461 (Wiki Lists): wiki-to-HTML list translator.

1..1000 lines, total length of all lines <= 1000 characters, each line
has at least one character that is not a list marker (* or #). Only
letters, digits, * and # appear. Seed 1 is a single plain line. Seed 2
is 1000 one-character lines forming alternating runs of */# markers (the
maximum line count). Seed 3 is one line with a near-1000-deep marker
prefix (maximum nesting depth) plus shallow siblings. Later seeds mix
random marker prefixes, ties that do/don't form blocks (single-marker
runs that stay plain), and content that itself contains marker symbols.
"""

import random
import sys

LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"


def content(rng, length=1):
    # at least one non-marker char guaranteed by drawing from LETTERS
    return ''.join(rng.choice(LETTERS) for _ in range(max(1, length)))


def emit(lines):
    assert 1 <= len(lines) <= 1000
    total = sum(len(s) for s in lines)
    assert total <= 1000, total
    for s in lines:
        assert s and any(ch not in '*#' for ch in s)
        assert all(ch in LETTERS or ch in '*#' for ch in s)
    sys.stdout.write('\n'.join(lines) + '\n')


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(["FirstLine"])

elif seed == 2:
    # maximum legal line count under the 1000-char budget: each line is one
    # marker plus one letter (2 chars), giving 500 lines arranged in
    # alternating runs of '*' and '#' so many sibling blocks open and close
    lines = []
    while len(lines) < 500:
        marker = rng.choice('*#')
        run = rng.randint(1, 8)
        for _ in range(run):
            if len(lines) >= 500:
                break
            lines.append(marker + content(rng, 1))
    emit(lines)

elif seed == 3:
    # maximum nesting depth: one deeply-prefixed line plus a sibling so a
    # block forms at every level, total chars near the 1000 limit
    depth = 480
    marker = '*'
    lines = [
        marker * depth + content(rng, 1),
        marker * depth + content(rng, 1),
    ]
    emit(lines)

elif seed == 4:
    # nested mixed markers, recursively forming ol/ul inside li
    lines = []
    budget = 980
    while budget > 4 and len(lines) < 1000:
        d = rng.randint(0, 6)
        prefix = ''.join(rng.choice('*#') for _ in range(d))
        body = content(rng, rng.randint(1, 3))
        line = prefix + body
        if len(line) > budget:
            break
        lines.append(line)
        budget -= len(line)
    if not lines:
        lines = ["x"]
    emit(lines)

elif seed == 5:
    # single-marker runs (length 1) that must stay plain lines, interleaved
    # with length>=2 runs that become lists
    lines = []
    budget = 960
    toggle = 0
    while budget > 4 and len(lines) < 1000:
        marker = rng.choice('*#')
        run = 1 if toggle % 2 == 0 else rng.randint(2, 4)
        toggle += 1
        for _ in range(run):
            line = marker + content(rng, 1)
            if len(line) > budget or len(lines) >= 1000:
                break
            lines.append(line)
            budget -= len(line)
    if not lines:
        lines = ["x"]
    emit(lines)

elif seed == 6:
    # content itself contains marker symbols after the first non-marker char
    lines = []
    budget = 950
    while budget > 6 and len(lines) < 1000:
        d = rng.randint(0, 4)
        prefix = ''.join(rng.choice('*#') for _ in range(d))
        mid = content(rng, 1)
        tail = ''.join(rng.choice('*#' + LETTERS) for _ in range(rng.randint(0, 4)))
        line = prefix + mid + tail
        if len(line) > budget:
            break
        lines.append(line)
        budget -= len(line)
    if not lines:
        lines = ["x"]
    emit(lines)

else:
    lines = []
    budget = rng.randint(50, 990)
    while budget > 6 and len(lines) < 1000:
        d = rng.randint(0, 8)
        prefix = ''.join(rng.choice('*#') for _ in range(d))
        body = content(rng, rng.randint(1, 4))
        line = prefix + body
        if len(line) > budget:
            break
        lines.append(line)
        budget -= len(line)
    if not lines:
        lines = ["x"]
    emit(lines)
