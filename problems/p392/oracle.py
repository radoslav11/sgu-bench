import sys
import random
import time

# We implement modulo M = 2^61 - 1 arithmetic (Mersenne prime),
# similar to the C++ solution, for fast-ish hashing.
M = (1 << 61) - 1

def mod_add(a: int, b: int) -> int:
    x = a + b
    x = (x & M) + (x >> 61)
    if x >= M:
        x -= M
    return x

def mod_sub(a: int, b: int) -> int:
    return mod_add(a, M - b)

def mod_mul(a: int, b: int) -> int:
    # Python has big integers, so we can multiply then reduce.
    # Reduce using Mersenne property:
    x = a * b
    x = (x & M) + (x >> 61)
    x = (x & M) + (x >> 61)
    if x >= M:
        x -= M
    return x

def mod_pow(a: int, e: int) -> int:
    res = 1
    while e > 0:
        if e & 1:
            res = mod_mul(res, a)
        a = mod_mul(a, a)
        e >>= 1
    return res

class HashMonoid:
    __slots__ = ("h", "p", "l")  # hash, power, length

    def __init__(self, h: int, p: int, l: int):
        self.h = h
        self.p = p
        self.l = l

    @staticmethod
    def identity():
        # Empty string: H=0, B^0=1, len=0
        return HashMonoid(0, 1, 0)

    @staticmethod
    def single(ch: str, base: int):
        # Single character: H=ord(ch), power=B, len=1
        return HashMonoid(ord(ch), base, 1)

    def merge(self, other: "HashMonoid") -> "HashMonoid":
        # Concatenation:
        # H = H1 * B^{len2} + H2
        # P = B^{len1+len2} = P1 * P2
        return HashMonoid(
            mod_add(mod_mul(self.h, other.p), other.h),
            mod_mul(self.p, other.p),
            self.l + other.l
        )

def geometric_series_sum(k: int, p: int) -> int:
    """
    Return 1 + p + p^2 + ... + p^(k-1) mod M, in O(log k),
    using the same doubling technique as the C++ solution.
    """
    if k == 0:
        return 0
    result = 0     # sum for selected blocks
    subsum = 1     # sum for current block length t: 1 + p + ... + p^(t-1)
    current_pow = p  # p^t

    while k > 0:
        if k & 1:
            # result = result * p^t + subsum
            result = mod_add(mod_mul(result, current_pow), subsum)
        # double the block:
        # subsum(2t) = subsum(t) * (1 + p^t)
        subsum = mod_mul(subsum, mod_add(current_pow, 1))
        current_pow = mod_mul(current_pow, current_pow)
        k >>= 1
    return result

def parse_compressed(query: str, base: int) -> HashMonoid:
    """
    Parse compressed query (no nested parentheses) and return HashMonoid
    of the fully expanded string, without expanding explicitly.
    """
    # Wrap in "(...)1" so we always reduce at the end.
    s = "(" + query + ")1"

    # Stack of HashMonoid; marker is l = -1
    st = []

    i = 0
    while i < len(s):
        ch = s[i]
        if ch == '(':
            st.append(HashMonoid(0, 1, -1))
            i += 1
        elif ch == ')':
            # Pop until marker, building the inside group.
            combined = HashMonoid.identity()
            while st and st[-1].l != -1:
                combined = st[-1].merge(combined)
                st.pop()
            st.pop()  # pop marker

            # Read repetition count
            i += 1
            cnt = 0
            while i < len(s) and s[i].isdigit():
                cnt = cnt * 10 + (ord(s[i]) - 48)
                i += 1

            # Repeat combined cnt times:
            pow_repeat = mod_pow(combined.p, cnt)
            h_repeat = mod_mul(combined.h, geometric_series_sum(cnt, combined.p))
            st.append(HashMonoid(h_repeat, pow_repeat, combined.l * cnt))
        else:
            # Regular character
            st.append(HashMonoid.single(ch, base))
            i += 1

    return st[-1]

def get_next_cell(arrows, r, c, x, y):
    a = arrows[x][y]
    nx, ny = x, y
    if a == '<':
        ny -= 1
    elif a == '>':
        ny += 1
    elif a == '^':
        nx -= 1
    else:  # 'v'
        nx += 1
    if nx < 0 or nx >= r or ny < 0 or ny >= c:
        return (-1, -1)
    return (nx, ny)

def compute_hash_from_pos(pos, steps, dp, jumps):
    """
    Using binary lifting, compute the HashMonoid of exactly `steps` letters
    read starting at pos, or fail if we exit the grid too early.
    """
    if steps == 0:
        return (HashMonoid.identity(), True)
    x, y = pos
    if x == -1:
        return (HashMonoid.identity(), False)

    res = HashMonoid.identity()
    # We need up to 2^30 for 1e9
    for bit in range(30, -1, -1):
        if steps & (1 << bit):
            if x == -1:
                return (HashMonoid.identity(), False)
            m = dp[bit][x][y]
            # If we couldn't build a full 2^bit segment (exited early), fail.
            if m.l < (1 << bit):
                return (HashMonoid.identity(), False)
            res = res.merge(m)
            x, y = jumps[bit][x][y]
    return (res, True)

def solve():
    input_data = sys.stdin.read().strip().splitlines()
    it = iter(input_data)
    r, c = map(int, next(it).split())
    arrows = [next(it).strip() for _ in range(r)]
    letters = [next(it).strip() for _ in range(r)]
    q = int(next(it))
    queries = [next(it).strip() for _ in range(q)]

    # Random base (non-zero preferred); keep it in [1, M-1]
    random.seed(time.time_ns())
    base = random.randrange(1, M)

    max_log = 31

    # jumps[bit][i][j] = (ni,nj) after 2^bit moves
    jumps = [[ [(-1, -1) for _ in range(c)] for _ in range(r) ] for _ in range(max_log)]
    # dp[bit][i][j] = HashMonoid of 2^bit letters from (i,j), or shorter if exits
    dp = [[ [None for _ in range(c)] for _ in range(r) ] for _ in range(max_log)]

    # Level 0
    for i in range(r):
        for j in range(c):
            jumps[0][i][j] = get_next_cell(arrows, r, c, i, j)
            dp[0][i][j] = HashMonoid.single(letters[i][j], base)

    # Build lifting tables
    for bit in range(1, max_log):
        for i in range(r):
            for j in range(c):
                ni, nj = jumps[bit - 1][i][j]
                if ni == -1:
                    # Can't extend beyond exiting the grid
                    jumps[bit][i][j] = (-1, -1)
                    dp[bit][i][j] = dp[bit - 1][i][j]
                else:
                    dp[bit][i][j] = dp[bit - 1][i][j].merge(dp[bit - 1][ni][nj])
                    jumps[bit][i][j] = jumps[bit - 1][ni][nj]

    # Parse all queries
    targets = [parse_compressed(s, base) for s in queries]

    out_lines = []
    for target in targets:
        best = None
        for i in range(r):
            for j in range(c):
                got, ok = compute_hash_from_pos((i, j), target.l, dp, jumps)
                if ok and got.h == target.h:
                    if best is None or (i, j) < best:
                        best = (i, j)
        if best is None:
            out_lines.append("NO")
        else:
            out_lines.append(f"YES ({best[0]+1},{best[1]+1})")

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
