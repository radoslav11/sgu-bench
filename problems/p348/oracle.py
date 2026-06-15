import sys

def rotate_left(x: int, k: int, length: int) -> int:
    """
    Cyclic left-rotate x by k within exactly 'length' bits.
    """
    mask = (1 << length) - 1
    k %= length
    return ((x << k) | (x >> (length - k))) & mask

def min_rotation(x: int, length: int) -> int:
    """
    Minimum rotation of x among rotations that keep the top bit set
    (i.e., result is still a 'length'-bit number).
    """
    best = x
    top_bit = 1 << (length - 1)
    for k in range(length):
        r = rotate_left(x, k, length)
        if r & top_bit:          # valid only if highest bit remains 1
            if r < best:
                best = r
    return best

def solve(n: int) -> int:
    # Bit-length of n (n>=1)
    length = n.bit_length()

    # Start with candidate x = n
    ans = min_rotation(n, length)

    # Enumerate boundary numbers u by flipping each 1-bit (except top bit) to 0
    # and making all lower bits 1.
    # pos = 0..length-2
    for pos in range(0, length - 1):
        if (n >> pos) & 1:
            # Clear bits 0..pos in n, keep higher bits
            upper = n & ~((1 << (pos + 1)) - 1)
            # Set lower bits 0..pos-1 to 1, keep bit pos as 0
            lower_ones = (1 << pos) - 1
            u = upper | lower_ones

            ans = max(ans, min_rotation(u, length))

    return ans

def main():
    n_str = sys.stdin.readline().strip()
    n = int(n_str)
    print(solve(n))

if __name__ == "__main__":
    main()
