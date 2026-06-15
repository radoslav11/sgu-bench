import sys

def solve_case(n1, m1, n2, m2) -> float:
    """
    Computes P(p < q | data) where:
      p|data ~ Beta(m1+1, n1-m1+1)
      q|data ~ Beta(m2+1, n2-m2+1)

    Uses the same O(n1) summation and recurrence as the C++ code,
    keeping everything as floating-point products (no factorials).
    """

    # term will hold the current summand T(j)
    term = 1.0

    # Compute C(n1+1, m1+1) multiplicatively:
    # C(N, K) = Π_{i=0}^{K-1} (N-i)/(i+1)
    # Here N = n1+1, K = m1+1 -> i from 0..m1
    for i in range(m1 + 1):
        term *= (n1 + 1 - i) / (i + 1)

    # Multiply by the Beta ratio that makes term equal to the first summand T(m1+1):
    # B(m1+m2+2, n1+n2-m1-m2+1) / B(m2+1, n2-m2+1)
    #
    # Expanded into products to avoid huge factorials.

    # Multiply by (m2+1)(m2+2)...(m2+m1+1)
    for i in range(1, m1 + 2):
        term *= (m2 + i)

    # Multiply by (n2-m2+1)(n2-m2+2)...(n2-m2+n1-m1)
    for i in range(1, n1 - m1 + 1):
        term *= (n2 - m2 + i)

    # Divide by (n2+2)(n2+3)...(n2+n1+2)
    for i in range(1, n1 + 2):
        term /= (n2 + 1 + i)

    # Start summation with the first term j = m1+1
    ans = term

    # Recurrence:
    # T(j+1)/T(j) = (n1+1-j)*(j+m2+1) / [(j+1)*(n1+n2-m2-j+1)]
    #
    # Loop j = m1+1 .. n1, producing terms up to j = n1+1.
    for j in range(m1 + 1, n1 + 1):
        term *= (n1 + 1 - j) * (j + m2 + 1)
        term /= (j + 1) * (n1 + n2 - m2 - j + 1)
        ans += term

    return ans


def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    t = int(data[0])
    idx = 1

    out_lines = []
    for _ in range(t):
        n1 = int(data[idx]); m1 = int(data[idx+1]); n2 = int(data[idx+2]); m2 = int(data[idx+3])
        idx += 4
        ans = solve_case(n1, m1, n2, m2)
        out_lines.append(f"{ans:.10f}")

    sys.stdout.write("\n".join(out_lines))


if __name__ == "__main__":
    main()
