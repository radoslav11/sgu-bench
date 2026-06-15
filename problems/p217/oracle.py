import sys
import math

def main():
    # Read two floating-point radii
    data = sys.stdin.read().strip().split()
    r1 = float(data[0])
    r2 = float(data[1])

    # Ensure r1 <= r2 (r1 is the smaller radius r, r2 is the larger radius R)
    if r1 > r2:
        r1, r2 = r2, r1

    # Integrand f(z) = sqrt((r^2 - z^2)(R^2 - z^2))
    def f(z: float) -> float:
        return math.sqrt((r1*r1 - z*z) * (r2*r2 - z*z))

    # Simpson's rule parameters.
    # n must be even; large n gives high accuracy for required 1e-4 output.
    n = 1_000_000
    h = r1 / n  # step size on [0, r1]

    # Simpson sum initialization with endpoints
    s = f(0.0) + f(r1)

    # Add interior points with alternating weights 4 and 2
    for i in range(1, n):
        z = i * h
        if i % 2 == 1:
            s += 4.0 * f(z)
        else:
            s += 2.0 * f(z)

    # Approximate integral_0^{r1} f(z) dz
    integral = s * h / 3.0

    # Total volume includes factor 8 from symmetry
    volume = 8.0 * integral

    # Print with 4 digits after decimal point
    sys.stdout.write(f"{volume:.4f}\n")

if __name__ == "__main__":
    main()
