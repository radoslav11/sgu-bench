"""Tests for p357 (Remote Control). Input size is fixed: 13 button flags
(0/1) plus channels X, Y in 0..99, so every test is already maximal-size.
Seeds vary the button mask and channel pair instead.

Seed plan:
- seed 1: everything works, X == Y (answer 0).
- seed 2: everything works, extreme channels 0 -> 99.
- seed 3: all buttons broken (answer -1 unless X == Y).
- seed 4: only Up works (long wrap-around walk).
- seed 5: only Down works.
- seed 6: only "--" and a couple of digits work.
- seed 7: "--" works but tens digits all broken (two-digit jumps dead).
- seeds 8+: random masks and random channels.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)


def emit(buttons, x, y):
    # buttons: dict with keys '0'..'9', 'up', 'down', 'dash'
    b = buttons
    print(b["1"], b["2"], b["3"], b["up"])
    print(b["4"], b["5"], b["6"], b["down"])
    print(b["7"], b["8"], b["9"])
    print(b["dash"], b["0"])
    print(x, y)


def mask(values):
    keys = [str(d) for d in range(10)] + ["up", "down", "dash"]
    return dict(zip(keys, values))


ALL_ON = mask([1] * 13)
ALL_OFF = mask([0] * 13)

if seed == 1:
    emit(ALL_ON, 57, 57)
elif seed == 2:
    emit(ALL_ON, 0, 99)
elif seed == 3:
    emit(ALL_OFF, 13, 87)
elif seed == 4:
    m = mask([0] * 13)
    m["up"] = 1
    emit(m, 99, 98)
elif seed == 5:
    m = mask([0] * 13)
    m["down"] = 1
    emit(m, 0, 1)
elif seed == 6:
    m = mask([0] * 13)
    m["dash"] = 1
    m["1"] = 1
    m["7"] = 1
    emit(m, 17, 71)
elif seed == 7:
    m = mask([0] * 13)
    m["dash"] = 1
    m["0"] = 1
    emit(m, 42, 50)
else:
    values = [rng.randint(0, 1) for _ in range(13)]
    m = mask(values)
    emit(m, rng.randint(0, 99), rng.randint(0, 99))
