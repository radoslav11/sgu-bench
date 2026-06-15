import sys

def main():
    data = sys.stdin.read().strip().split()
    # Scaling factor for four decimal places
    B = 10000

    # Read and scale L
    Lf = float(data[0])
    L = int(Lf * B + 0.5)      # Convert to integer

    # Number of intervals
    N = int(data[1])

    # Parse T_i and V_i
    idx = 2
    s = 0  # scaled total distance mod L
    for _ in range(N):
        T = int(data[idx]); V = int(data[idx+1])
        idx += 2
        # Add T*V*B, reduce modulo L immediately
        # (T * V) may be large, but Python int handles it
        s = (s + T * V * B) % L

    # Shortest arc: either s forward or (L - s) backward
    best = min(s, L - s)

    # Output with four decimals
    # Divide by B to restore original scale
    print(f"{best/ B:.4f}")

if __name__ == "__main__":
    main()
