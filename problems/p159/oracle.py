import sys

sys.setrecursionlimit(10000)


def solve():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    b = int(data[0])
    n = int(data[1])

    carry = [0] * n
    x = []
    ans = []

    def pop_carry(pos):
        while pos < n and carry[pos] >= b:
            full = carry[pos] // b
            carry[pos] %= b
            pos += 1
            if pos < n:
                carry[pos] += full

    def rec(pos):
        if pos == n:
            s = []
            for d in reversed(x):
                if d >= 10:
                    s.append(chr(ord("A") + d - 10))
                else:
                    s.append(chr(ord("0") + d))
            ans.append("".join(s))
            return

        carry_save = carry[:]
        start_digit = 1 if (pos == n - 1 and n != 1) else 0

        for d in range(start_digit, b):
            x.append(d)
            factor = 1 if pos == 0 else 2
            if (carry[pos] + factor * x[0] * x[pos]) % b != x[pos]:
                x.pop()
                continue

            for j in range(pos + 1):
                pair_factor = 1 if j == pos else 2
                val = pair_factor * x[pos] * x[j]
                idx = pos + j
                if idx < n:
                    carry[idx] += val
                    pop_carry(idx)

            if carry[pos] == x[pos]:
                rec(pos + 1)

            for i in range(n):
                carry[i] = carry_save[i]
            x.pop()

    rec(0)

    out_lines = [str(len(ans))]
    out_lines.extend(ans)
    sys.stdout.write("\n".join(out_lines))


if __name__ == "__main__":
    solve()
