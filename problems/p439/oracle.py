import sys
import random
import time

# We'll implement rolling hash with mod = 2^61 - 1 (same modulus idea as C++).
# Python integers are arbitrary precision, so we can implement safe reduction.

MOD = (1 << 61) - 1

def mod_add(a, b):
    x = a + b
    x = (x & MOD) + (x >> 61)
    if x >= MOD:
        x -= MOD
    return x

def mod_sub(a, b):
    return mod_add(a, MOD - b)

def mod_mul(a, b):
    # Multiply then reduce mod (2^61-1) using the identity:
    # x mod (2^61-1) = (x & MOD) + (x >> 61), repeated once or twice.
    x = a * b
    x = (x & MOD) + (x >> 61)
    x = (x & MOD) + (x >> 61)
    if x >= MOD:
        x -= MOD
    return x

class RollingHash:
    def __init__(self, max_len):
        # Random base in [1..MOD-2]
        seed = int(time.time_ns()) ^ random.getrandbits(64)
        rng = random.Random(seed)
        self.base = rng.randrange(1, MOD - 1)

        # Precompute powers of base up to max_len
        self.pow = [1] * (max_len + 1)
        for i in range(1, max_len + 1):
            self.pow[i] = mod_mul(self.pow[i - 1], self.base)

    def prefix_hash(self, s):
        # Build prefix hashes:
        # h[i+1] = hash(s[0..i]) (length i+1)
        h = [0] * (len(s) + 1)
        for i, ch in enumerate(s):
            h[i + 1] = mod_add(mod_mul(h[i], self.base), ord(ch))
        return h

    def range_hash(self, h, l, r):
        # hash of s[l:r] (half-open)
        # = h[r] - h[l] * base^(r-l)
        return mod_sub(h[r], mod_mul(h[l], self.pow[r - l]))

def solve():
    data = sys.stdin.read().strip().split()
    n, m = map(int, data[:2])
    A = data[2].strip()
    B = data[3].strip()

    # Double strings to model cyclic shifts as substrings
    A2 = A + A
    B2 = B + B

    # Prepare hashing (need powers up to 2*n, safe for all queries we do)
    rh = RollingHash(2 * n + 5)
    ha = rh.prefix_hash(A2)
    hb = rh.prefix_hash(B2)

    def lcp_a_b(i, j, max_len):
        """LCP between A2[i:] and B2[j:] up to max_len."""
        lo, hi = 0, max_len
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if rh.range_hash(ha, i, i + mid) == rh.range_hash(hb, j, j + mid):
                lo = mid
            else:
                hi = mid - 1
        return lo

    def lcp_b_b(i, j, max_len):
        """LCP between B2[i:] and B2[j:] up to max_len."""
        lo, hi = 0, max_len
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if rh.range_hash(hb, i, i + mid) == rh.range_hash(hb, j, j + mid):
                lo = mid
            else:
                hi = mid - 1
        return lo

    # Compare two cyclic shifts of B lexicographically
    def cmp_shift(i, j):
        l = lcp_b_b(i, j, m)
        if l == m:
            return False
        return B2[i + l] < B2[j + l]

    # Step 1: lexicographically smallest cyclic shift of B
    best_b = 0
    for i in range(1, m):
        if cmp_shift(i, best_b):
            best_b = i

    # Step 2: check if A2[pos:pos+m] differs from B2[best_b:best_b+m] by <= 1 char
    def mismatches_le_1(pos):
        l1 = lcp_a_b(pos, best_b, m)
        if l1 == m:
            return True  # 0 mismatches
        if l1 == m - 1:
            return True  # exactly 1 mismatch (last char)
        # skip one mismatch at l1, then remaining must match
        l2 = lcp_a_b(pos + l1 + 1, best_b + l1 + 1, m - l1 - 1)
        return (l1 + 1 + l2 == m)

    # Choose shift of A with minimal shift count; tie -> left shift.
    best_a = None
    for d in range(0, n // 2 + 1):
        # left shift by d => start index d
        if mismatches_le_1(d):
            best_a = d
            break
        # right shift by d => start index n-d (avoid duplicates)
        if d > 0 and n - d != d and mismatches_le_1(n - d):
            best_a = n - d
            break

    # Output results
    result_b = B2[best_b:best_b + m]
    result_a = A2[best_a:best_a + n]
    sys.stdout.write(result_b + "\n" + result_a + "\n")

if __name__ == "__main__":
    solve()
