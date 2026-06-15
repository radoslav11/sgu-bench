def find_shortest_absent(s):
    n = len(s)
    # Map characters to bits 0 or 1
    bits = [ord(c) - ord('a') for c in s]

    # Try each length L = 1, 2, 3, ...
    L = 1
    while True:
        # There are 2^L possible patterns; track which ones we see
        size = 1 << L
        seen = [False] * size

        # Build initial mask from first L bits
        mask = 0
        for i in range(L):
            mask = (mask << 1) | bits[i]
        seen[mask] = True

        # Slide window over the rest; maintain rolling hash in 'mask'
        full_mask = size - 1
        for i in range(L, n):
            mask = ((mask << 1) & full_mask) | bits[i]
            seen[mask] = True

        # Find first mask not seen
        for m in range(size):
            if not seen[m]:
                # reconstruct the string of length L
                res = []
                for b in range(L - 1, -1, -1):
                    bit = (m >> b) & 1
                    res.append(chr(bit + ord('a')))
                return L, ''.join(res)

        # not found at this length, try next
        L += 1

if __name__ == "__main__":
    import sys
    data = sys.stdin.read().split()
    n = int(data[0])
    s = data[1].strip()
    length, keyword = find_shortest_absent(s)
    print(length)
    print(keyword)
