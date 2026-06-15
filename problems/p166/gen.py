"""Tests for p166 (Editor): one command line, <= 20480 commands, every
text line stays <= 1000 chars at all times.

The generator simulates the editor itself and only emits a command when
the line-length guarantee survives it. Seed 1 is just "Q". Seeds 2-3 are
maximal 20480-command scripts (insert-heavy long lines vs. split-heavy
many lines). Later seeds stress merges (X/B at line ends), far-right
cursor padding, and sample-style trailing junk after Q.
"""

import random
import sys

MAXLEN = 1000
MAXCMD = 20480
LETTERS = "abcdefghijklmnopqrstuvwxyz "


class Editor:
    def __init__(self):
        self.lines = [""]
        self.row, self.col = 0, 0

    def fits(self, ch):
        """Would applying ch keep every line at <= MAXLEN chars?"""
        s = self.lines[self.row]
        if ch in LETTERS:
            return max(len(s), self.col) + 1 <= MAXLEN
        if ch == "X" and self.col >= len(s) and self.row + 1 < len(self.lines):
            return self.col + len(self.lines[self.row + 1]) <= MAXLEN
        if ch == "B" and self.col == 0 and self.row > 0:
            return len(self.lines[self.row - 1]) + len(s) <= MAXLEN
        return True

    def apply(self, ch):
        lines, row, col = self.lines, self.row, self.col
        s = lines[row]
        if ch in LETTERS:
            if col > len(s):
                s = s + " " * (col - len(s))
            lines[row] = s[:col] + ch + s[col:]
            self.col += 1
        elif ch == "L":
            self.col = max(0, col - 1)
        elif ch == "R":
            self.col += 1
        elif ch == "U":
            self.row = max(0, row - 1)
        elif ch == "D":
            self.row = min(len(lines) - 1, row + 1)
        elif ch == "N":
            col = min(col, len(s))
            lines[row] = s[:col]
            lines.insert(row + 1, s[col:])
            self.row, self.col = row + 1, 0
        elif ch == "E":
            self.col = len(s)
        elif ch == "H":
            self.col = 0
        elif ch == "X":
            if col < len(s):
                lines[row] = s[:col] + s[col + 1:]
            elif row + 1 < len(lines):
                lines[row] = s + " " * (col - len(s)) + lines[row + 1]
                del lines[row + 1]
        elif ch == "B":
            if col > 0:
                if col <= len(s):
                    lines[row] = s[:col - 1] + s[col:]
                self.col -= 1
            elif row > 0:
                self.col = len(lines[row - 1])
                lines[row - 1] += s
                del lines[row]
                self.row -= 1


def script(rng, n_cmds, weights):
    ed = Editor()
    cmds = []
    pool = [c for c, w in weights.items() for _ in range(w)]
    while len(cmds) < n_cmds - 1:
        ch = rng.choice(pool)
        if ch == "a":
            ch = rng.choice(LETTERS)
        if ch == "R" and ed.col > MAXLEN + 50:
            continue
        if not ed.fits(ch):
            continue
        ed.apply(ch)
        cmds.append(ch)
    cmds.append("Q")
    return "".join(cmds)


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print("Q")
elif seed == 2:
    # maximal script biased toward typing: few long lines near 1000 chars
    w = {"a": 60, "L": 4, "R": 6, "U": 2, "D": 2, "N": 1, "E": 2, "H": 2,
         "X": 4, "B": 4}
    print(script(rng, MAXCMD, w))
elif seed == 3:
    # maximal script biased toward splits/merges: thousands of short lines
    w = {"a": 12, "L": 4, "R": 4, "U": 8, "D": 8, "N": 12, "E": 4, "H": 4,
         "X": 10, "B": 10}
    print(script(rng, MAXCMD, w))
elif seed == 4:
    # far-right cursor abuse: R runs beyond end, then letters / X padding
    w = {"a": 20, "L": 2, "R": 40, "U": 4, "D": 4, "N": 6, "E": 2, "H": 4,
         "X": 12, "B": 8}
    print(script(rng, MAXCMD, w))
elif seed == 5:
    # delete-heavy: text rarely survives, lots of no-op edge commands
    w = {"a": 10, "L": 6, "R": 6, "U": 6, "D": 6, "N": 6, "E": 4, "H": 4,
         "X": 25, "B": 25}
    print(script(rng, 8000, w))
elif seed == 6:
    # sample-style: trailing junk after Q must be ignored
    w = {"a": 30, "L": 3, "R": 5, "U": 3, "D": 3, "N": 5, "E": 3, "H": 3,
         "X": 5, "B": 5}
    body = script(rng, 500, w)
    junk = "".join(rng.choice(LETTERS) for _ in range(200))
    print(body + junk)
else:
    w = {c: rng.randint(1, 20) for c in "aLRUDNEHXB"}
    print(script(rng, rng.randint(50, MAXCMD), w))
