"""Tests for p138 (Games of Chess): N <= 100 friends, total games <= 10000.

Every test is produced by simulating a random valid chain of games
(winner of game i plays in game i+1) and printing the per-friend game
counts, so a schedule always exists. Seed 1 is minimal (N=2, one game),
seeds 2-3 are maximal (G=10000) with different winner dynamics.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    n, games = 2, 1
    stay_prob = 0.5
elif seed == 2:
    n, games = 100, 10000
    stay_prob = 0.5
elif seed == 3:
    # one dominant champion keeps winning almost every game
    n, games = 100, 10000
    stay_prob = 0.99
elif seed == 4:
    # only two friends, they trade wins for the whole series
    n, games = 2, 10000
    stay_prob = 0.5
elif seed == 5:
    # few games among many friends -> lots of friends with 0 games
    n, games = 100, 60
    stay_prob = 0.3
else:
    n = rng.randint(2, 100)
    games = rng.randint(1, 10000)
    stay_prob = rng.choice([0.1, 0.3, 0.5, 0.8, 0.95])

counts = [0] * (n + 1)
winner, opponent = rng.sample(range(1, n + 1), 2)
counts[winner] += 1
counts[opponent] += 1
if rng.random() >= stay_prob:
    winner = opponent

for _ in range(games - 1):
    opponent = rng.randint(1, n - 1)
    if opponent >= winner:
        opponent += 1
    counts[winner] += 1
    counts[opponent] += 1
    if rng.random() >= stay_prob:
        winner = opponent

print(n)
print(" ".join(str(counts[i]) for i in range(1, n + 1)))
