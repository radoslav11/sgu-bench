"""Tests for p288 (Best Tournament Schedule): 1 <= N <= 2005.

Input is just N. Seed 1 is the minimal N=1, seeds 2-3 are the maximal
odd/even sizes, the rest cover tiny and random mid-size values.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(2005)
elif seed == 3:
    print(2004)
elif seed == 4:
    print(2)
elif seed == 5:
    print(3)
else:
    print(rng.randint(4, 2003))
