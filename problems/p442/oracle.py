import sys

sys.set_int_max_str_digits(1000000)


def count_solutions(S, expecting_carry_initial):
    n = len(S)
    if n == 0:
        return 0

    dp = [[0, 0], [0, 0]]
    dp[expecting_carry_initial][0] = 1

    left, right = 0, n - 1
    while left <= right:
        d_left = ord(S[left]) - ord('0')
        d_right = ord(S[right]) - ord('0')
        is_middle = (left == right)

        ndp = [[0, 0], [0, 0]]

        for exp_carry in (0, 1):
            for carry_r in (0, 1):
                ways = dp[exp_carry][carry_r]
                if ways == 0:
                    continue

                for a in range(10):
                    if left == 0 and a == 0:
                        continue

                    if is_middle:
                        b_range = (a,)
                    else:
                        b_range = range(10)

                    for b in b_range:
                        s = a + b
                        right_val = s + carry_r
                        if right_val % 10 != d_right:
                            continue
                        new_carry_r = right_val // 10

                        new_exp_carry = d_left + 10 * exp_carry - s
                        if new_exp_carry < 0 or new_exp_carry > 1:
                            continue

                        if is_middle:
                            if new_carry_r == exp_carry:
                                ndp[0][0] += ways
                        else:
                            ndp[new_exp_carry][new_carry_r] += ways

        dp = ndp
        left += 1
        right -= 1

    return dp[0][0] + dp[1][1]


def solve_one(N):
    ans = count_solutions(N, 0)
    if len(N) > 1 and N[0] == '1':
        ans += count_solutions(N[1:], 1)
    return ans


def main():
    out_lines = []
    for line in sys.stdin:
        N = line.strip()
        if not N:
            continue
        if N == "0":
            break
        out_lines.append(str(solve_one(N)))
    sys.stdout.write("\n".join(out_lines) + ("\n" if out_lines else ""))


if __name__ == "__main__":
    main()
