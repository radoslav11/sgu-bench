import sys
import math

def main():
    data = sys.stdin.read().strip().split()
    n, m = map(int, data)

    # Edge case: only one box ⇒ exactly one prize
    if n == 1:
        print("1.0")
        return

    # Using the closed‐form expectation:
    # E = N * (1 - ((N-1)/N)^M)
    # Compute (N-1)/N once
    ratio = (n - 1) / n
    # pow(ratio, m) is ((N-1)/N)^M
    p_never = ratio**m
    expected = n * (1 - p_never)

    # Output with sufficient precision
    print(f"{expected:.10f}")

if __name__ == "__main__":
    main()
