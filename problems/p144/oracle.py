import sys

def main():
    # Read input: X and Y are integers (hours), Z is a float (minutes)
    data = sys.stdin.read().strip().split()
    X, Y = map(int, data[:2])
    Z = float(data[2])

    # Convert the total meeting window (Y-X) from hours to minutes
    delta = (Y - X) * 60.0

    # If Z >= delta, they will always meet (the waiting window covers the whole interval)
    # but the formula below handles that case naturally (it gives P = 1).
    # Compute probability using P = (2*delta*Z - Z^2) / delta^2
    numerator = 2.0 * delta * Z - Z * Z
    denominator = delta * delta
    probability = numerator / denominator

    # Print the result with 7 decimal places
    sys.stdout.write(f"{probability:.7f}\n")

if __name__ == "__main__":
    main()
