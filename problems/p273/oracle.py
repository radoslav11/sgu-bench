import sys


def color_id(c):
    if c == 'b':
        return 0
    if c == 'r':
        return 1
    if c == 'y':
        return 2
    return 3


def main():
    data = sys.stdin.read().split()
    ptr = 0
    cnt = list(map(int, data[ptr:ptr + 4]))
    ptr += 4

    # rule_mask[a][b] is a 4-bit mask of colors that can be produced
    # by replacing adjacent stones of colors a followed by b.
    rule_mask = [[0] * 4 for _ in range(4)]
    for result_color in range(4):
        for _ in range(cnt[result_color]):
            rule = data[ptr]
            ptr += 1
            a = color_id(rule[0])
            b = color_id(rule[1])
            rule_mask[a][b] |= 1 << result_color

    row = data[ptr]
    n = len(row)

    # transition[left_mask][right_mask] gives all colors obtainable by
    # merging one color from left_mask with one color from right_mask.
    transition = [[0] * 16 for _ in range(16)]
    for left_mask in range(16):
        for right_mask in range(16):
            result_mask = 0
            for a in range(4):
                if not (left_mask & (1 << a)):
                    continue
                for b in range(4):
                    if not (right_mask & (1 << b)):
                        continue
                    result_mask |= rule_mask[a][b]
            transition[left_mask][right_mask] = result_mask

    # dp[l][r] is a 4-bit mask of colors into which row[l..r] can reduce.
    dp = [[0] * n for _ in range(n)]
    for i, ch in enumerate(row):
        dp[i][i] = 1 << color_id(ch)

    for length in range(2, n + 1):
        for l in range(0, n - length + 1):
            r = l + length - 1
            mask = 0
            dpl = dp[l]
            for mid in range(l, r):
                mask |= transition[dpl[mid]][dp[mid + 1][r]]
                if mask == 15:
                    break
            dpl[r] = mask

    colors = "bryw"
    final_mask = dp[0][n - 1]
    answer = [colors[c] for c in range(4) if final_mask & (1 << c)]
    print("".join(answer) if answer else "Nobody")


if __name__ == "__main__":
    main()
