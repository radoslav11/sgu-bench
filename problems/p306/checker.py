"""Checker for p306: usage checker.py <in> <out> <ans>; exit 0 = accept.

The first line must claim the same optimal number of weighings as the
reference. The rest is any valid decision script: every weighing puts
equally many distinct coins on disjoint pans, every scenario (fake coin
i, lighter or heavier) follows existing branches to a leaf "fake i"
within the claimed number of weighings, and every described case branch
is reachable by at least one scenario (no impossible cases).
"""

import re
import sys


def reject(message):
    print(message)
    sys.exit(1)


def parse_side(text, n):
    text = text.replace(" ", "")
    if not re.fullmatch(r"\d+(\+\d+)*", text):
        reject(f"malformed pan '{text}'")
    coins = [int(x) for x in text.split("+")]
    if any(not (1 <= c <= n) for c in coins):
        reject(f"coin out of range in pan '{text}'")
    if len(set(coins)) != len(coins):
        reject(f"duplicate coin in pan '{text}'")
    return coins


class Node:
    def __init__(self):
        self.fake = None
        self.left = None
        self.right = None
        self.branches = {}  # '<' / '=' / '>' -> Node
        self.used = set()


def parse_node(lines, pos, n):
    if pos >= len(lines):
        reject("unexpected end of script")
    line = lines[pos]
    node = Node()

    m = re.fullmatch(r"fake\s+(\d+)", line)
    if m:
        node.fake = int(m.group(1))
        if not (1 <= node.fake <= n):
            reject(f"fake coin {node.fake} out of range")
        return node, pos + 1

    m = re.fullmatch(r"weigh\s+(.*\S)\s+vs\s+(.*\S)", line)
    if not m:
        reject(f"expected 'weigh'/'fake', got '{line}'")
    node.left = parse_side(m.group(1), n)
    node.right = parse_side(m.group(2), n)
    if len(node.left) != len(node.right):
        reject(f"unequal pans in '{line}'")
    if set(node.left) & set(node.right):
        reject(f"coin on both pans in '{line}'")

    pos += 1
    while pos < len(lines) and lines[pos] != "end":
        m = re.fullmatch(r"case\s*([<=>])\s*:", lines[pos])
        if not m:
            reject(f"expected 'case X:' or 'end', got '{lines[pos]}'")
        sym = m.group(1)
        if sym in node.branches:
            reject(f"duplicate case {sym}")
        child, pos = parse_node(lines, pos + 1, n)
        node.branches[sym] = child
    if pos >= len(lines):
        reject("missing 'end'")
    if not node.branches:
        reject("weighing with no cases")
    return node, pos + 1


def simulate(root, fake, delta, max_depth):
    node, depth = root, 0
    while node.fake is None:
        depth += 1
        if depth > max_depth:
            reject(f"scenario fake={fake} needs more than {max_depth} weighings")
        diff = 0
        if fake in node.left:
            diff = delta
        elif fake in node.right:
            diff = -delta
        sym = "<" if diff < 0 else (">" if diff > 0 else "=")
        if sym not in node.branches:
            reject(f"scenario fake={fake} delta={delta}: missing case {sym}")
        node.used.add(sym)
        node = node.branches[sym]
    if node.fake != fake:
        reject(f"scenario fake={fake} delta={delta}: script answers {node.fake}")


def check_reachable(node):
    if node.fake is not None:
        return
    for sym, child in node.branches.items():
        if sym not in node.used:
            reject(f"impossible case {sym} described for weigh {node.left} vs {node.right}")
        check_reachable(child)


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    ans_first = open(sys.argv[3]).readline().split()
    expected_w = int(ans_first[1])

    lines = [ln.strip() for ln in open(sys.argv[2]).read().splitlines()]
    lines = [ln for ln in lines if ln]
    if not lines:
        reject("empty output")

    m = re.fullmatch(r"need\s+(\d+)\s+weighings", lines[0])
    if not m:
        reject(f"bad first line '{lines[0]}'")
    if int(m.group(1)) != expected_w:
        reject(f"need {expected_w} weighings, claimed {m.group(1)}")

    root, pos = parse_node(lines, 1, n)
    if pos != len(lines):
        reject("trailing garbage after script")
    if root.fake is not None:
        reject("script is a single leaf")

    for fake in range(1, n + 1):
        for delta in (-1, 1):
            simulate(root, fake, delta, expected_w)
    check_reachable(root)
    sys.exit(0)


if __name__ == "__main__":
    main()
