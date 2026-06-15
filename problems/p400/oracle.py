import sys
from dataclasses import dataclass
from typing import List


@dataclass
class Team:
    # Team name.
    name: str

    # Number of solved problems in the frozen standings.
    solved: int

    # Frozen penalty.
    penalty: int

    # Frozen rank.
    rank: int

    # For every unsolved problem, store the number of previous wrong submissions.
    rejects_unsolved: List[int]


def better_than(a: Team, b: Team) -> bool:
    """
    Return True if team a is strictly better than team b.
    """
    if a.solved != b.solved:
        return a.solved > b.solved
    return a.penalty < b.penalty


def clone_teams(teams: List[Team]) -> List[Team]:
    """
    Make a deep-enough copy of teams.
    The only mutable field is rejects_unsolved, so copy that list.
    """
    return [
        Team(t.name, t.solved, t.penalty, t.rank, t.rejects_unsolved[:])
        for t in teams
    ]


def accept_one(t: Team) -> None:
    """
    Simulate one accepted problem for team t during the last hour.

    To minimize penalty, choose the currently unsolved problem with the fewest
    previous rejected submissions.
    """
    # Find index of cheapest unsolved problem.
    idx = min(range(len(t.rejects_unsolved)), key=lambda i: t.rejects_unsolved[i])

    # Number of previous rejected submissions on this problem.
    rejects = t.rejects_unsolved[idx]

    # One more solved problem.
    t.solved += 1

    # Earliest possible accepted submission is at minute 241.
    t.penalty += 241 + 20 * rejects

    # Remove this problem from the unsolved list.
    t.rejects_unsolved.pop(idx)


def parse_team_row(line: str, n_problems: int) -> Team:
    """
    Parse one standings row.

    Since team names may contain spaces, parsing is done from the right:
    problem results, penalty, solved count are all at the right side.
    """
    last = len(line) - 1

    # Skip trailing whitespace.
    while last >= 0 and line[last].isspace():
        last -= 1

    def skip_spaces():
        """
        Move last left over whitespace.
        Python closure uses nonlocal to modify last.
        """
        nonlocal last
        while last >= 0 and line[last].isspace():
            last -= 1

    def read_digits_back() -> int:
        """
        Read a nonnegative integer while moving from right to left.
        """
        nonlocal last
        value = 0
        base = 1

        while last >= 0 and line[last].isdigit():
            value += base * int(line[last])
            base *= 10
            last -= 1

        return value

    rejects_unsolved = []

    # Parse all problem result columns from right to left.
    for _ in range(n_problems):
        skip_spaces()

        # Optional number attached to '+' or '-' result.
        # For '.', this will usually be 0.
        num = read_digits_back()

        skip_spaces()

        # Result marker: '+', '-', or '.'
        sign = line[last]
        last -= 1

        # If the problem was not solved before freeze, it may be solved later.
        if sign != '+':
            rejects_unsolved.append(num)

    skip_spaces()

    # Parse frozen penalty.
    penalty = read_digits_back()

    skip_spaces()

    # Parse frozen solved count.
    solved = read_digits_back()

    # Now parse rank from the left.
    s = 0

    # Skip leading whitespace.
    while s < len(line) and line[s].isspace():
        s += 1

    rank = 0

    # Rank is the first integer in the row.
    while s < len(line) and line[s].isdigit():
        rank = rank * 10 + int(line[s])
        s += 1

    # Skip spaces after rank.
    while s < len(line) and line[s].isspace():
        s += 1

    # Trim trailing spaces before solved count.
    while last >= s and line[last].isspace():
        last -= 1

    # Everything between rank and solved-count area is the team name.
    name = line[s:last + 1]

    return Team(name, solved, penalty, rank, rejects_unsolved)


def is_team_row(line: str) -> bool:
    """
    A team row contains problem result symbols.
    The balloon-count line does not.
    """
    return any(c in ".+-" for c in line)


def high_pos(original_teams: List[Team], my_idx: int, balloons: int, n_problems: int) -> int:
    """
    Compute best possible rank for our team.
    """
    teams = clone_teams(original_teams)

    me = teams[my_idx]
    balloons_left = balloons

    # Give our team as many accepted problems as possible.
    while balloons_left > 0 and me.rejects_unsolved:
        accept_one(me)
        balloons_left -= 1

    n = len(teams)

    # Decide whether leftover balloons can be assigned elsewhere harmlessly.
    if my_idx != n - 1:
        absorbed_elsewhere = True
    elif my_idx == 0:
        absorbed_elsewhere = True
    else:
        absorbed_elsewhere = teams[my_idx - 1].solved < n_problems

    # If yes, they do not affect our rank.
    if absorbed_elsewhere:
        balloons_left = 0

    # Edge handling: unavoidable extra penalty.
    if balloons_left > 0:
        me.penalty += 20 * balloons_left

    # Initially assume every team above us still beats us.
    rank = my_idx + 1

    # Move up while teams above no longer beat us.
    i = my_idx - 1
    while i >= 0 and not better_than(teams[i], me):
        rank -= 1
        i -= 1

    return rank


def low_pos(original_teams: List[Team], my_idx: int, balloons: int) -> int:
    """
    Compute worst possible rank for our team.
    """
    teams = clone_teams(original_teams)
    n = len(teams)

    # Put our team at the beginning of its equal-rank block.
    # Tied teams have equal solved and penalty, so swapping is harmless.
    i = my_idx - 1
    while i >= 0 and teams[i].rank == teams[my_idx].rank:
        if i == 0 or teams[i - 1].rank != teams[my_idx].rank:
            teams[i], teams[my_idx] = teams[my_idx], teams[i]
            my_idx = i
            break
        i -= 1

    me = teams[my_idx]

    # Start from frozen rank.
    rank = me.rank

    balloons_left = balloons

    # Start processing teams below us.
    i = my_idx + 1

    while balloons_left > 0 and i < n:
        j = i

        # Find a group with the same solved count.
        while i < n and teams[i].solved == teams[j].solved:
            i += 1

        # This group needs det solves to reach our solved count.
        det = me.solved - teams[j].solved

        # If not enough balloons to let even one team reach us, stop.
        if balloons_left < det:
            break

        # Simulate cheapest det solves for every team in this group.
        for l in range(j, i):
            for _ in range(det):
                accept_one(teams[l])

        # First, teams that beat us after exactly det solves.
        for l in range(j, i):
            if balloons_left >= det and better_than(teams[l], me):
                balloons_left -= det
                rank += 1

        # Then, teams that need one extra solve to beat us by solved count.
        for l in range(j, i):
            if balloons_left >= det + 1:
                if not better_than(teams[l], me) and teams[l].rejects_unsolved:
                    balloons_left -= det + 1
                    accept_one(teams[l])
                    rank += 1

    return rank


def solve_all() -> None:
    """
    Read all test cases and print answers.
    """
    lines = sys.stdin.read().splitlines()
    ptr = 0
    out = []

    def read_nonempty_line():
        """
        Return next non-empty line, skipping blank lines.
        """
        nonlocal ptr

        while ptr < len(lines):
            line = lines[ptr]
            ptr += 1

            if any(not c.isspace() for c in line):
                return line

        return None

    while True:
        # Read header.
        header = read_nonempty_line()

        if header is None:
            break

        tokens = header.split()

        # Header has four fixed tokens before problem identifiers:
        # Rank Team = Penalty
        n_problems = len(tokens) - 4

        teams = []

        # Read team rows until balloon count.
        while True:
            line = read_nonempty_line()

            if line is None:
                return

            if not is_team_row(line):
                balloons = int(line.strip())
                break

            teams.append(parse_team_row(line, n_problems))

        # Read our team name.
        my_team = read_nonempty_line()

        if my_team is None:
            return

        my_team = my_team.strip()

        # Find our team.
        my_idx = -1

        for idx, team in enumerate(teams):
            if team.name == my_team:
                my_idx = idx
                break

        # Compute best and worst ranks.
        best = high_pos(teams, my_idx, balloons, n_problems)
        worst = low_pos(teams, my_idx, balloons)

        out.append(f"{best} {worst}")

    print("\n".join(out))


if __name__ == "__main__":
    solve_all()
