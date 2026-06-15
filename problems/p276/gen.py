"""Tests for p276 (Andrew's Troubles): 0 <= S, P <= 10^4.

Seed 1 is the minimal input, seeds 2-3 the extreme corners, the
remaining seeds probe the 1 / 300 / 900 / 1800 second thresholds and
random pairs.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(0, 0)
elif seed == 2:
    print(0, 10000)
elif seed == 3:
    print(10000, 0)
elif seed == 4:
    # exactly at each cup threshold relative to S = 1000
    print(1000, 1000 + rng.choice([1, 300, 900, 1800]))
elif seed == 5:
    # one second before a threshold
    print(1000, 1000 + rng.choice([299, 899, 1799]))
elif seed == 6:
    print(10000, 10000)
else:
    print(rng.randint(0, 10000), rng.randint(0, 10000))
