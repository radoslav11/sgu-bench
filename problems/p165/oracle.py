import sys

def parse_micrometers(s):
    """
    Parse a decimal string s representing meters into integer micrometers.
    Assumes input heights are integer micrometers as per the statement.
    Examples:
      "2"    -> 2_000_000
      "1.95" -> 1_950_000
      "2.050000" -> 2_050_000
    """
    s = s.strip()
    neg = s.startswith('-')
    if neg:
        s = s[1:]
    if '.' in s:
        left, right = s.split('.', 1)
    else:
        left, right = s, ''
    # pad or truncate fractional part to 6 digits (micrometers)
    right = (right + '000000')[:6]
    microunits = int(left) * 1_000_000 + (int(right) if right else 0)
    return -microunits if neg else microunits

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    heights = [next(it) for _ in range(n)]

    # Convert to integer micrometers and then to deviations from 2_000_000 μm (2.0 m)
    two_m_micros = 2_000_000
    deviations = []
    for idx, s in enumerate(heights):
        mic = parse_micrometers(s)
        deviations.append((mic - two_m_micros, idx + 1))  # store (deviation, 1-based index)

    # Sort by deviation ascending
    deviations.sort(key=lambda t: t[0])

    # Two-pointer greedy: keep prefix sum in [-50_000, 50_000]
    S = 0  # prefix sum of deviations, in micrometers
    l, r = 0, n - 1
    order = []
    for _ in range(n):
        if S > 0:
            d, idx = deviations[l]
            l += 1
        else:
            d, idx = deviations[r]
            r -= 1
        S += d
        order.append(idx)

    # Under problem guarantees, a solution always exists
    out = []
    out.append("yes")
    out.append(" ".join(map(str, order)))
    print("\n".join(out))

if __name__ == "__main__":
    main()
