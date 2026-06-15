import math
import sys

def solve():
    # Read h (height), d (bisector), m (median)
    data = sys.stdin.read().strip().split()
    h_in, d_in, m_in = map(int, data)

    # Feasibility condition for this construction:
    # Need h <= d <= m, otherwise xD or xM are not consistent with the geometry.
    if h_in > d_in or d_in > m_in:
        print("NO")
        return

    # Boundary cases:
    # If d == h or m == d, the general formula divides by xD or yields t=0.
    # The only valid non-degenerate case here is h == d == m (isosceles).
    if h_in == d_in or d_in == m_in:
        if h_in == d_in == m_in:
            h = float(h_in)
            print("YES")
            # A at (0,h), B and C at (-h,0),(h,0)
            print(f"{0:.8f} {h:.8f}")
            print(f"{-h:.8f} {0:.8f}")
            print(f"{h:.8f} {0:.8f}")
        else:
            print("NO")
        return

    # General case h < d < m
    h = float(h_in)
    d = float(d_in)
    m = float(m_in)

    # Put A=(0,h), BC on x-axis, H=(0,0).
    # From AM^2 = xM^2 + h^2 => xM = sqrt(m^2 - h^2)
    xM = math.sqrt(m*m - h*h)

    # From AD^2 = xD^2 + h^2 => xD = sqrt(d^2 - h^2)
    xD = math.sqrt(d*d - h*h)

    # Derived from angle bisector theorem under symmetric parametrization:
    # t^2 = (xM - xD) * (h^2 + xM*xD) / xD
    t2 = (xM - xD) * (h*h + xM*xD) / xD
    # Due to floating error, clamp very small negatives to zero
    if t2 < 0 and t2 > -1e-12:
        t2 = 0.0
    if t2 <= 0:
        # Would mean degenerate triangle (area 0) or impossible
        print("NO")
        return
    t = math.sqrt(t2)

    # Construct points
    Ax, Ay = 0.0, h
    Bx, By = xM - t, 0.0
    Cx, Cy = xM + t, 0.0

    print("YES")
    print(f"{Ax:.8f} {Ay:.8f}")
    print(f"{Bx:.8f} {By:.8f}")
    print(f"{Cx:.8f} {Cy:.8f}")

if __name__ == "__main__":
    solve()
