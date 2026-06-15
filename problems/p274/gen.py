"""Tests for p274 (Spam-filter): N <= 100 addresses, each 1..100 chars
with byte values 32..255.

Seed 1 is minimal (one 1-char address). Seed 2 is maximal: 100 lines of
100 chars, a mix of valid emails padded to full length and one-byte
mutations of valid emails. Seed 3 is maximal with random bytes 32..255
(high bytes emitted as UTF-8 pairs whose bytes are also in 32..255, so
the file stays decodable by the harness). Other seeds mix valid
emails, near-miss mutations (double dots, bad domain length, two '@',
leading/trailing dots, spaces) and short garbage.
"""

import random
import sys

seed = int(sys.argv[1])
rng = random.Random(seed)

LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
SYMBOLS = LETTERS + "0123456789_-"


def word(max_len):
    return "".join(rng.choice(SYMBOLS) for _ in range(rng.randint(1, max_len)))


def valid_email(target_len=None):
    while True:
        prefix = ".".join(word(6) for _ in range(rng.randint(1, 4)))
        sprefix = ".".join(word(6) for _ in range(rng.randint(1, 4)))
        domain = "".join(rng.choice(LETTERS) for _ in range(rng.choice((2, 3))))
        email = f"{prefix}@{sprefix}.{domain}"
        if len(email) <= 100:
            break
    if target_len is not None and len(email) < target_len:
        pad = target_len - len(email) - 1
        if pad > 0:
            email = word(1) * 0 + ("".join(rng.choice(SYMBOLS) for _ in range(pad))) + "." + email
            email = email[-target_len:]
            if email[0] == '.' or email[0] == '@':
                email = rng.choice(SYMBOLS) + email[1:]
    return email[:100]


def mutate(email):
    ops = rng.randint(1, 2)
    s = list(email)
    for _ in range(ops):
        op = rng.randrange(5)
        pos = rng.randrange(len(s))
        if op == 0:
            s[pos] = chr(rng.randint(32, 126))
        elif op == 1:
            s.insert(pos, rng.choice(".@ "))
        elif op == 2 and len(s) > 1:
            del s[pos]
        elif op == 3:
            s[pos] = "."
        else:
            s[pos] = "@"
    return "".join(s)[:100]


def random_line(length):
    # byte values 32..255 are allowed; high bytes are emitted as UTF-8
    # pairs (each byte of the pair is still in 32..255) so the line stays
    # decodable while `length` counts BYTES, capped at 100
    chunks = []
    used = 0
    while used < length:
        if rng.random() < 0.15 and used + 2 <= length:
            chunks.append(chr(rng.randint(0xA0, 0xFF)))
            used += 2
        else:
            chunks.append(chr(rng.randint(32, 126)))
            used += 1
    return "".join(chunks)


lines = []
if seed == 1:
    lines = ["a"]
elif seed == 2:
    for i in range(100):
        if i % 2 == 0:
            lines.append(valid_email(100))
        else:
            lines.append(mutate(valid_email(100)))
elif seed == 3:
    lines = [random_line(100) for _ in range(100)]
else:
    n = rng.randint(1, 100)
    for _ in range(n):
        kind = rng.randrange(6)
        if kind == 0:
            lines.append(valid_email())
        elif kind == 1:
            lines.append(mutate(valid_email()))
        elif kind == 2:
            lines.append(random_line(rng.randint(1, 100)))
        elif kind == 3:
            edge = rng.choice([
                "a@bc.de", "_@-.ru", "a@b.cd", "@ab.cd", "a@.cd",
                "a@bc.d", "a@bc.defg", "a..b@cd.ef", "a@b@c.de",
                "a@bc.de ", " a@bc.de", "a b@cd.ef", "a@cd.e9",
                "A@BC.DE", "9@99.aa", "-_-@-_-.aa", "a@a.aa.",
                ".a@bc.de", "a.@bc.de", "a@bc..de",
            ])
            lines.append(edge[:100])
        elif kind == 4:
            lines.append(valid_email().swapcase())
        else:
            lines.append(mutate(mutate(valid_email())))

out = str(len(lines)) + "\n" + "\n".join(lines) + "\n"
sys.stdout.buffer.write(out.encode("utf-8"))
