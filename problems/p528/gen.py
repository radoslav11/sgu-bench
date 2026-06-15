"""Tests for p528 (Bencoding): m in [2,1e9], sequence of ASCII 33..127.

The sequence length is 1..1e6. Seed 1 is the minimal valid object "0:"
with m=2. Seeds 2-3 are maximal ~1e6-char sequences: seed 2 a deeply nested
valid object, seed 3 a long flat list of small items. Later seeds cover
invalid/truncated inputs, leading-zero traps, huge string-length fields,
the m-too-small case, and random near-valid sequences.
"""

import random
import sys

ASCII_LO, ASCII_HI = 33, 127
LEN_CAP = 10**6


def rand_char(rng):
    return chr(rng.randint(ASCII_LO, ASCII_HI))


def gen_object(rng, depth, budget):
    """Build a random valid bencoded object; returns a string.

    budget bounds the produced length loosely to keep within LEN_CAP.
    """
    if depth <= 0 or budget < 4:
        choice = rng.choice(["str", "int"])
    else:
        choice = rng.choice(["str", "int", "list", "dict", "str", "int"])

    if choice == "int":
        if rng.random() < 0.2:
            return "i0e"
        digits = rng.randint(1, min(15, max(1, budget - 2)))
        n = str(rng.randint(1, 9))
        n += "".join(str(rng.randint(0, 9)) for _ in range(digits - 1))
        return "i" + n + "e"

    if choice == "str":
        maxlen = max(0, min(budget - 4, 30))
        k = rng.randint(0, maxlen) if maxlen > 0 else 0
        body = "".join(rand_char(rng) for _ in range(k))
        return f"{k}:{body}"

    if choice == "list":
        n_items = rng.randint(0, 4)
        parts = []
        b = budget - 2
        for _ in range(n_items):
            if b < 4:
                break
            item = gen_object(rng, depth - 1, b // max(1, n_items))
            parts.append(item)
            b -= len(item)
        return "l" + "".join(parts) + "e"

    # dict
    n_pairs = rng.randint(0, 3)
    parts = []
    b = budget - 2
    for _ in range(n_pairs):
        if b < 8:
            break
        key = gen_object(rng, depth - 1, b // (max(1, n_pairs) * 2))
        val = gen_object(rng, depth - 1, b // (max(1, n_pairs) * 2))
        parts.append(key + val)
        b -= len(key) + len(val)
    return "d" + "".join(parts) + "e"


def emit(m, c):
    sys.stdout.write(f"{m}\n{c}\n")


seed = int(sys.argv[1])
rng = random.Random(seed)

if seed == 1:
    emit(2, "0:")
    sys.exit()

if seed == 2:
    # Maximal deeply nested valid object close to 1e6 chars.
    # Nest lists/dicts ~ (1e6)/2 deep then close them all.
    depth = (LEN_CAP - 4) // 2
    s = "l" * depth + "0:" + "e" * depth
    if len(s) > LEN_CAP:
        depth = (LEN_CAP - 2) // 2
        s = "l" * depth + "e" * depth
    emit(len(s), s)
    sys.exit()

if seed == 3:
    # Maximal long flat list of small integers "i1e" repeated.
    inner = "i1e"
    count = (LEN_CAP - 2) // len(inner)
    s = "l" + inner * count + "e"
    s = s[:LEN_CAP]
    # ensure well-formed: trim to last complete and re-close
    count = (LEN_CAP - 2) // len(inner)
    s = "l" + inner * count + "e"
    emit(len(s), s)
    sys.exit()

if seed == 4:
    # Long string body near 1e6 (single big string object).
    k = LEN_CAP - 10
    body = "".join(rand_char(rng) for _ in range(k))
    s = f"{k}:{body}"
    emit(len(s) + 100, s)
    sys.exit()

if seed == 5:
    # m too small for a valid object that the sequence represents.
    obj = gen_object(rng, 4, 200)
    emit(rng.randint(2, max(2, len(obj) - 1)), obj)
    sys.exit()

if seed == 6:
    # Adversarial: leading zeros, negative ints, bad terminators.
    bad = rng.choice([
        "i-1e", "i01e", "01:x", "i e", "le e", "d1:ae", "i123",
        "3:ab", "2:abc", "00:", "i00e", "lli1ee",
    ])
    emit(rng.randint(2, 1000000000), bad)
    sys.exit()

if seed == 7:
    # Huge string-length field (many digits) -> length far exceeds m.
    digits = rng.randint(20, 50)
    n = str(rng.randint(1, 9)) + "".join(str(rng.randint(0, 9)) for _ in range(digits - 1))
    s = f"{n}:" + "".join(rand_char(rng) for _ in range(50))
    emit(rng.randint(2, 1000000000), s)
    sys.exit()

if seed == 8:
    # A valid object with a stray trailing character (invalid as a whole).
    obj = gen_object(rng, 5, 5000)
    s = obj + rand_char(rng)
    emit(rng.randint(len(obj), 1000000000), s)
    sys.exit()

# Random near-valid / random sequences of moderate length.
mode = rng.random()
if mode < 0.5:
    obj = gen_object(rng, 6, rng.randint(50, 20000))
    # randomly corrupt
    if rng.random() < 0.5 and len(obj) > 1:
        pos = rng.randint(0, len(obj) - 1)
        obj = obj[:pos] + rand_char(rng) + obj[pos + 1:]
    emit(rng.randint(2, 1000000000), obj)
else:
    length = rng.randint(1, 5000)
    s = "".join(rng.choice("ilde0123456789:abc") for _ in range(length))
    emit(rng.randint(2, 1000000000), s)
