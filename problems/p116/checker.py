"""Checker for p116: usage checker.py <in> <out> <ans>; exit 0 = accept.

The index I (minimal number of summands, or 0) is deterministic and
must match the reference answer. The presentation is a certificate:
exactly I super-primes (primes whose 1-based index among primes is
itself prime), listed in non-increasing order and summing to N. Any
such presentation is accepted.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def super_primes(limit):
    sieve = bytearray([1]) * (limit + 1)
    sieve[0] = sieve[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if sieve[i]:
            sieve[i * i::i] = bytearray(len(sieve[i * i::i]))
    primes = [i for i in range(limit + 1) if sieve[i]]
    sps = set()
    for idx, p in enumerate(primes, start=1):
        if idx <= limit and idx < len(sieve) and sieve[idx]:
            sps.add(p)
    return sps


def main():
    n = int(open(sys.argv[1]).read().split()[0])
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    expected_i = int(ans_tokens[0])

    if not out_tokens:
        reject("empty output")
    try:
        got_i = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be the index, got '{out_tokens[0]}'")
    if got_i != expected_i:
        reject(f"index is {expected_i}, claimed {got_i}")

    if expected_i == 0:
        if len(out_tokens) > 1:
            reject("trailing output after index 0")
        sys.exit(0)

    if len(out_tokens) != 1 + expected_i:
        reject(f"expected {expected_i} summands, got {len(out_tokens) - 1}")
    try:
        items = [int(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-integer summand")

    sps = super_primes(10000)
    for v in items:
        if v not in sps:
            reject(f"{v} is not a super-prime")
    if any(a < b for a, b in zip(items, items[1:])):
        reject("summands not in non-increasing order")
    if sum(items) != n:
        reject(f"summands add to {sum(items)}, expected {n}")

    sys.exit(0)


if __name__ == "__main__":
    main()
