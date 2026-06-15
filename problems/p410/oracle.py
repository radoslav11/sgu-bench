import sys

def solve() -> None:
    data = sys.stdin.read().strip().split()
    n = int(data[0])
    a = list(map(int, data[1:1+n]))

    # M = maximum initial mistkafers.
    # At least M flying days are required to reduce that planet to 0.
    M = max(a)
    ans = M  # start with M flights, add science days as needed

    # If ans <= 1000 we must output a schedule.
    # We'll still compute operations, but only store those we might print.
    pre_ops = []  # planets cloned before any flights (1-based indices)

    # ops[s] = list of planets to clone after exactly s flights (i.e., before flight s+1)
    # Only meaningful when M is small; cap for safety.
    ops_cap = min(M, 1024)
    ops = [[] for _ in range(ops_cap)]

    for i, x0 in enumerate(a):
        x = x0
        # While planet's current value is below M, we need some doublings.
        while x < M:
            # If we fly s days, values become x-s and M-s.
            # One clone then makes this planet 2*(x-s). We want it to equal M-s:
            # 2*(x-s) = M-s  ->  s = 2x - M
            s = 2 * x - M

            # If s is feasible (0 <= s <= x), one scheduled clone at time s solves it.
            if 0 <= s <= x:
                ans += 1
                # Store only if it can be printed in a <=1000-day plan.
                if s <= 1000 and s < ops_cap:
                    ops[s].append(i + 1)  # store 1-based planet index
                break

            # Otherwise x is too small; clone immediately (before flights) and try again.
            pre_ops.append(i + 1)
            ans += 1
            x *= 2

    out_lines = [str(ans)]

    # If answer is small, output a concrete plan:
    if ans <= 1000:
        # First, all early science missions.
        for p in pre_ops:
            out_lines.append(f"science mission to the planet {p}")

        # Then M times: (optional scheduled science missions) + flying.
        for day in range(M):
            if day < ops_cap:
                for p in ops[day]:
                    out_lines.append(f"science mission to the planet {p}")
            out_lines.append("flying mission")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
