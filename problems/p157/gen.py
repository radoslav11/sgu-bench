"""Tests for p157 (Patience): n is a single integer in 1..13.

There are only 13 possible inputs, so seed k simply prints n = k
(seed 1 is the minimum, seed 13 the maximum).
"""

import sys

seed = int(sys.argv[1])
print(seed)
