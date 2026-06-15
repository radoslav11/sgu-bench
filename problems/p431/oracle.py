import sys

sys.setrecursionlimit(10**7)


def match_classic(pattern, name):
    px = 0
    nx = 0
    last_star_px = -1
    next_nx_after_star = -1

    while px < len(pattern) or nx < len(name):
        if px < len(pattern):
            c = pattern[px]
            if c == '?':
                if nx < len(name):
                    px += 1
                    nx += 1
                    continue
            elif c == '*':
                last_star_px = px
                next_nx_after_star = nx + 1
                px += 1
                continue
            else:
                if nx < len(name) and name[nx] == c:
                    px += 1
                    nx += 1
                    continue

        if last_star_px != -1 and 0 <= next_nx_after_star <= len(name):
            px = last_star_px
            nx = next_nx_after_star
            next_nx_after_star += 1
            continue

        return False

    return True


def solve():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])
    a = data[2:2 + n + m]

    base = min(a[:n], key=len)

    def check_all(p):
        for i, s in enumerate(a):
            should_match = (i < n)
            if match_classic(p, s) != should_match:
                return False
        return True

    def rec(p, suff):
        if not suff and check_all(p):
            return True, p
        if check_all(p + "*"):
            return True, p + "*"
        if not suff:
            return False, ""

        for i in range(len(suff)):
            c = suff[i]
            new_suff = suff[i + 1:]
            options = ["*" + c]
            if i == 0:
                options.append(c)
                options.append("?")
            for mid in options:
                ok, ans = rec(p + mid, new_suff)
                if ok:
                    return True, ans

        return False, ""

    ok, ans = rec("", base)
    sys.stdout.write((ans if ok else "OOPS") + "\n")


if __name__ == "__main__":
    solve()
