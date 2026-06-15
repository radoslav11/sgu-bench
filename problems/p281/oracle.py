"""Second oracle for p281: block scan of ranking 1 against positions in
ranking 2; whenever the prefix sets coincide a block ends and is sorted
alphabetically."""

import sys


def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    ord1 = [next(it) for _ in range(n)]
    ord2 = [next(it) for _ in range(n)]

    pos2 = {name: i for i, name in enumerate(ord2)}

    answer = []
    block = []
    max_pos2 = -1
    for i, name in enumerate(ord1):
        block.append(name)
        max_pos2 = max(max_pos2, pos2[name])
        if max_pos2 == i:
            block.sort()
            answer.extend(block)
            block = []

    if block:
        block.sort()
        answer.extend(block)

    sys.stdout.write("\n".join(answer) + "\n")


main()
