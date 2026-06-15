"""Tests for p442 (X + R(X) = N): count positive X with X + reverse(X) = N.

Input is a sequence of decimal numbers N, one per line, terminated by a
single '0'. The all-nines N maximizes both the number of solutions and the
size of the bignum answer, so it is the heaviest input for the reference's
O(|N|) digit DP with big-integer counts.

Seed 1 is the minimal input (the smallest N with a solution, N = 2). Seeds
2 and 3 are maximal: a single ~100000-digit all-nines N, even length in
seed 2 and odd length in seed 3 (the odd length exercises the middle-digit
branch). Later seeds bundle many smaller cases, including leading-'1'
numbers that trigger the |X| = |N|-1 case, palindromic N, and random N.
"""

import random
import sys

BIG = 100000

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(numbers):
    for s in numbers:
        print(s)
    print(0)


if seed == 1:
    emit(["2"])
elif seed == 2:
    emit(["9" * BIG])
elif seed == 3:
    emit(["9" * (BIG + 1)])
elif seed == 4:
    # several medium all-nines numbers of mixed length
    nums = ["9" * rng.randint(1000, 6000) for _ in range(8)]
    emit(nums)
elif seed == 5:
    # leading-'1' numbers that can trigger the |X| = |N| - 1 case
    nums = []
    for _ in range(20):
        length = rng.randint(2, 40)
        body = "".join(rng.choice("0123456789") for _ in range(length - 1))
        nums.append("1" + body)
    emit(nums)
elif seed == 6:
    # palindromic N of various lengths
    nums = []
    for _ in range(30):
        half = "".join(rng.choice("123456789") for _ in range(rng.randint(1, 6)))
        if rng.random() < 0.5:
            nums.append(half + half[::-1])
        else:
            mid = rng.choice("0123456789")
            nums.append(half + mid + half[::-1])
    emit(nums)
else:
    nums = []
    count = rng.randint(50, 300)
    for _ in range(count):
        length = rng.randint(1, 60)
        first = rng.choice("123456789")
        rest = "".join(rng.choice("0123456789") for _ in range(length - 1))
        nums.append(first + rest)
    emit(nums)
