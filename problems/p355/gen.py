"""Tests for p355 (Numbers Painting): one integer N per test.

The statement's N bound is lost ("integer number N ()"); the reference
handles up to 2^20 but exceeds the 64 MB memory limit there, so the
package caps N at 2^17 = 131072 (reference: ~36 MB, 0.04 s). Seed 1 is
the minimal N=1, seeds 2-3 are the maximal power of two and the value
just below it (which changes the optimal color count), the rest mix
powers of two, primes and random sizes.
"""

import random
import sys

MAX_N = 131072

seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    print(1)
elif seed == 2:
    print(MAX_N)
elif seed == 3:
    print(MAX_N - 1)
elif seed == 4:
    print(2)
elif seed == 5:
    print(65536)  # 2^16, exact power of two boundary
elif seed == 6:
    print(65537)  # prime just above a power of two
elif seed == 7:
    print(12)  # the statement's sample
else:
    print(rng.randint(2, MAX_N))
