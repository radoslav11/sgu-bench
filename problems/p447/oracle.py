import sys


class Cmd:
    """
    Represents one valid rest command.
    """

    def __init__(self, val, length, string):
        # Duration in units of 1/64.
        self.val = val

        # Number of characters in this command.
        self.len = length

        # Actual command string, e.g. "R4..".
        self.str = string


def generate_commands():
    """
    Generate all valid single rest commands.

    There are 28 commands total:
    R1 with 0..6 dots,
    R2 with 0..5 dots,
    ...
    R64 with 0 dots.
    """

    # Possible base denominators.
    bases = [1, 2, 4, 8, 16, 32, 64]

    # Corresponding durations in 1/64 units.
    unit_vals = [64, 32, 16, 8, 4, 2, 1]

    cmds = []

    # Generate every base and all legal dotted versions.
    for base, unit_value in zip(bases, unit_vals):
        # Command without dots.
        s = "R" + str(base)

        # Current added duration part.
        half = unit_value

        # Total duration of this command.
        total = half

        # Add command without dots.
        cmds.append(Cmd(total, len(s), s))

        # Add dotted versions while the next dot still contributes at least 1/64.
        while half > 1:
            # Dot contributes half of the previous contribution.
            half //= 2

            # Increase command duration.
            total += half

            # Add dot to string.
            s += "."

            # Store this dotted command.
            cmds.append(Cmd(total, len(s), s))

    return cmds


def parse_total_value(commands):
    """
    Parse the input string and compute total duration in 1/64 units.
    """

    n = len(commands)
    i = 0
    total = 0

    # Parse command by command.
    while i < n:
        # Current character is 'R'.
        i += 1

        # Parse duration base number.
        base = 0
        while i < n and commands[i].isdigit():
            base = base * 10 + int(commands[i])
            i += 1

        # Convert base duration to 1/64 units.
        unit = 64 // base

        # Command duration starts with base unit.
        subtotal = unit

        # Parse dots.
        while i < n and commands[i] == ".":
            # Each dot adds half of the previous part.
            unit //= 2

            # Add this dot's contribution.
            subtotal += unit

            # Move past dot.
            i += 1

        # Add command duration to total.
        total += subtotal

    return total


def is_strict_prefix(a, b):
    """
    Return True if a is a strict prefix of b.
    """

    return len(a) < len(b) and b.startswith(a)


def compute_dp(V, cmds):
    """
    Compute dp[v] = minimum number of characters needed
    to represent duration v.
    """

    INF = 10**18

    # dp[v] is the shortest length for duration v.
    dp = [INF] * (V + 1)

    # Duration zero is represented by the empty string.
    dp[0] = 0

    # Compute in increasing order.
    for v in range(1, V + 1):
        best = INF

        # Try every command as the last command.
        for c in cmds:
            if c.val <= v:
                prev = dp[v - c.val]

                # If previous duration is reachable, update best.
                if prev != INF:
                    cand = prev + c.len
                    if cand < best:
                        best = cand

        dp[v] = best

    return dp


def pick_best_command(v, cmds, dp):
    """
    Choose the lexicographically best first command among all commands
    that can start an optimal solution for duration v.
    """

    feasible = []

    # Find all commands preserving the optimal length.
    for idx, c in enumerate(cmds):
        if c.val <= v and dp[v - c.val] + c.len == dp[v]:
            feasible.append(idx)

    survivors = []

    # Remove commands that are strict prefixes of another feasible command.
    for i in feasible:
        dominated = False

        for j in feasible:
            if i != j and is_strict_prefix(cmds[i].str, cmds[j].str):
                dominated = True
                break

        if not dominated:
            survivors.append(i)

    # Among survivors, ordinary string comparison decides.
    best = survivors[0]

    for i in survivors:
        if cmds[i].str < cmds[best].str:
            best = i

    return best


def solve():
    # Read the single input line.
    commands = sys.stdin.readline().strip()

    # Generate all possible rest commands.
    cmds = generate_commands()

    # Compute total input duration.
    V = parse_total_value(commands)

    # Compute shortest lengths.
    dp = compute_dp(V, cmds)

    # Reconstruct lexicographically smallest shortest answer.
    ans = []
    v = V

    while v > 0:
        # Pick the best next command.
        idx = pick_best_command(v, cmds, dp)

        # Append command string.
        ans.append(cmds[idx].str)

        # Subtract command duration.
        v -= cmds[idx].val

    # Print final expression.
    print("".join(ans))


if __name__ == "__main__":
    solve()
