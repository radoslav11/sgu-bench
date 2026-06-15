"""Tests for p400 (The last hour of the contest): frozen ACM standings,
balloons handed out in the last hour, min/max final place of one team.

Each dataset is built from scratch: every team gets a per-problem
history (no submit / -x rejects / accept at some minute with x prior
rejects), penalty and solved counts are derived from it, teams are
sorted and ranked with shared places, and the table is printed with
1-3 spaces between tokens. Total rejects per table stay <= 1000 and
the whole file under 100 KB.

Seed 1 is the minimal single-team set, seed 2 packs four maximal
100-team x 26-problem sets, seed 3 packs hundreds of tiny sets. Later
seeds focus on full tie blocks, everything-solved tables (leftover
balloons cannot be absorbed) and random mixes.
"""

import random
import string
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

NAME_CHARS = string.ascii_letters + string.digits


def make_names(count):
    names = set()
    out = []
    while len(out) < count:
        words = [
            "".join(rng.choice(NAME_CHARS)
                    for _ in range(rng.randint(1, 9)))
            for _ in range(rng.randint(1, 4))
        ]
        name = " ".join(words)
        if name not in names and len(name) <= 100:
            names.add(name)
            out.append(name)
    return out


def make_dataset(n_teams, n_problems, p_solve, p_reject, tie_blocks=False,
                 all_solved=False, balloons_mode="random"):
    lines = []
    reject_budget = 1000
    teams = []
    names = make_names(n_teams)
    proto = None
    for t in range(n_teams):
        solved = 0
        penalty = 0
        results = []
        if tie_blocks and proto is not None and rng.random() < 0.7:
            solved, penalty, results = proto
        else:
            results = []
            for _ in range(n_problems):
                r = rng.random()
                if all_solved or r < p_solve:
                    x = rng.randint(0, 3) if reject_budget >= 3 else 0
                    reject_budget -= x
                    minute = rng.randint(1, 240)
                    solved += 1
                    penalty += minute + 20 * x
                    results.append("+" + (str(x) if x else ""))
                elif r < p_solve + p_reject and reject_budget > 0:
                    x = rng.randint(1, min(4, reject_budget))
                    reject_budget -= x
                    results.append(f"-{x}")
                else:
                    results.append(".")
            proto = (solved, penalty, list(results))
        teams.append([names[t], solved, penalty, results])

    teams.sort(key=lambda tm: (-tm[1], tm[2]))
    ranks = []
    for i, tm in enumerate(teams):
        if i and (tm[1], tm[2]) == (teams[i - 1][1], teams[i - 1][2]):
            ranks.append(ranks[-1])
        else:
            ranks.append(i + 1)

    def sp():
        return " " * rng.randint(1, 3)

    header = "Rank Team = Penalty " + " ".join(
        string.ascii_uppercase[:n_problems]
    )
    lines.append(header)
    for i, (name, solved, penalty, results) in enumerate(teams):
        lines.append(
            sp().join([str(ranks[i]), name, str(solved), str(penalty)]
                      + results)
        )

    slots = n_teams * n_problems - sum(tm[1] for tm in teams)
    if balloons_mode == "zero":
        balloons = 0
    elif balloons_mode == "max":
        balloons = slots
    else:
        balloons = rng.randint(0, slots) if slots else 0
    lines.append(str(balloons))
    lines.append(rng.choice(teams)[0])
    return "\n".join(lines)


def emit_until(budget, dataset_factory):
    total = 0
    chunks = []
    while True:
        ds = dataset_factory()
        if total + len(ds) + 1 > budget:
            break
        chunks.append(ds)
        total += len(ds) + 1
    print("\n".join(chunks))


if seed == 1:
    print("Rank Team = Penalty A")
    print("1 lonely 0 0 .")
    print("0")
    print("lonely")
elif seed == 2:
    emit_until(
        98000,
        lambda: make_dataset(100, 26, p_solve=0.35, p_reject=0.25),
    )
elif seed == 3:
    emit_until(
        98000,
        lambda: make_dataset(
            rng.randint(1, 5), rng.randint(1, 4),
            p_solve=0.5, p_reject=0.3,
            tie_blocks=rng.random() < 0.5,
            balloons_mode=rng.choice(["zero", "max", "random"]),
        ),
    )
elif seed == 4:
    # one big block of fully tied teams
    print(make_dataset(100, 10, p_solve=0.4, p_reject=0.3,
                       tie_blocks=True, balloons_mode="random"))
elif seed == 5:
    # everybody solved everything: leftover balloons cannot be absorbed
    print(make_dataset(50, 8, p_solve=1.0, p_reject=0.0,
                       all_solved=True, balloons_mode="zero"))
    print(make_dataset(50, 8, p_solve=0.97, p_reject=0.03,
                       balloons_mode="max"))
else:
    emit_until(
        90000,
        lambda: make_dataset(
            rng.randint(1, 100), rng.randint(1, 26),
            p_solve=rng.uniform(0.1, 0.9),
            p_reject=rng.uniform(0.0, 0.5),
            tie_blocks=rng.random() < 0.3,
            balloons_mode=rng.choice(["zero", "max", "random", "random"]),
        ),
    )
