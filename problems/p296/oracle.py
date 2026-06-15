"""Independent oracle for p296 (remove K digits to maximize the number).

Classic monotonic-stack greedy: keep len(N)-K digits in order, popping a
smaller digit whenever a larger one can take its place and deletions
remain. O(|N|). Structurally distinct from the reference's candidate-set
sweep.
"""

import sys

data = sys.stdin.read().split()
n, k = data[0], int(data[1])
keep = len(n) - k
stack = []
removals = k
for c in n:
    while stack and removals > 0 and stack[-1] < c:
        stack.pop()
        removals -= 1
    stack.append(c)

print("".join(stack[:keep]))
