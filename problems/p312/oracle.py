import sys
from dataclasses import dataclass
from typing import List, Tuple

EPS = 1e-9

@dataclass
class Point:
    x: float
    y: float

    # Vector ops
    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, k: float): return Point(self.x * k, self.y * k)  # scalar
    def __truediv__(self, k: float): return Point(self.x / k, self.y / k)

def cross(a: Point, b: Point) -> float:
    """2D cross product of vectors a and b."""
    return a.x * b.y - a.y * b.x

def ccw(a: Point, b: Point, c: Point) -> int:
    """Orientation of triangle (a,b,c): +1 ccw, 0 collinear, -1 cw."""
    v = cross(b - a, c - a)
    if -EPS <= v <= EPS:
        return 0
    return 1 if v > 0 else -1

def tri_area(a: Point, b: Point, c: Point) -> float:
    """Signed area of triangle (a,b,c)."""
    return 0.5 * cross(b - a, c - a)

def parse_ratios(s: str) -> List[int]:
    """Parse '1:3:1:3' -> [1,3,1,3]."""
    return list(map(int, s.strip().split(":")))

# Region is (cnt, [points...])
Region = Tuple[int, List[Point]]

def make_tri(a: Point, b: Point, c: Point) -> Region:
    return (3, [a, b, c])

def make_quad(a: Point, b: Point, c: Point, d: Point) -> Region:
    return (4, [a, b, c, d])

def split_quad(v0: Point, v1: Point, v2: Point, v3: Point,
               r1: float, r2: float) -> Tuple[Region, Region]:
    """
    Split quadrilateral v0-v1-v2-v3 into two regions with area ratio r1:r2
    by cutting from v0 to some point on chain v1->v2->v3.
    """
    a1 = tri_area(v0, v1, v2)
    a2 = tri_area(v0, v2, v3)
    total = a1 + a2

    target = (r1 / (r1 + r2)) * total

    # Try placing cut point on segment v1->v2
    s = target / a1
    if -1e-9 <= s <= 1 + 1e-9:
        s = max(0.0, min(1.0, s))
        P = v1 + (v2 - v1) * s
        return make_tri(v0, v1, P), make_quad(v0, P, v2, v3)

    # Otherwise on segment v2->v3
    rem = target - a1
    s2 = rem / a2
    s2 = max(0.0, min(1.0, s2))
    P = v2 + (v3 - v2) * s2
    return make_quad(v0, v1, v2, P), make_tri(v0, P, v3)

def fmt_point(p: Point) -> str:
    # Print with many digits; judge accepts any reasonable precision.
    return f"{p.x:.10f} {p.y:.10f}"

def print_region(region: Region) -> str:
    cnt, pts = region
    parts = [str(cnt)]
    for p in pts:
        parts.append(fmt_point(p))
    return " ".join(parts)

def solve(data: str) -> str:
    it = iter(data.strip().split())
    n = int(next(it))
    pts = [Point(float(next(it)), float(next(it))) for _ in range(n)]
    # ratio string may have ':' so it is tokenized as one word already
    ratio_str = next(it)
    K = parse_ratios(ratio_str)

    out: List[str] = []

    if n == 3:
        A, B, C = pts
        s = K[0] + K[1] + K[2]

        # Same barycentric construction as C++:
        # O = (K2*A + K3*B + K1*C) / sum
        O = (A * K[1] + B * K[2] + C * K[0]) / s

        out.append(print_region(make_tri(A, B, O)))
        out.append(print_region(make_tri(B, C, O)))
        out.append(print_region(make_tri(C, A, O)))
        return "\n".join(out)

    # n == 4
    A, B, C, D = pts
    s = sum(K)

    s1 = ccw(A, C, B)
    s2 = ccw(A, C, D)

    r = [None] * 4  # type: ignore

    if s1 * s2 < 0:
        # Use diagonal AC
        t = (K[0] + K[3]) / s
        Q = A + (C - A) * t

        r0, r3 = split_quad(A, B, Q, D, K[0], K[3])
        r2, r1 = split_quad(C, D, Q, B, K[2], K[1])

        r[0], r[1], r[2], r[3] = r0, r1, r2, r3
    else:
        # Use diagonal BD
        t = (K[0] + K[1]) / s
        Q = B + (D - B) * t

        r1, r0 = split_quad(B, C, Q, A, K[1], K[0])
        r3, r2 = split_quad(D, A, Q, C, K[3], K[2])

        r[0], r[1], r[2], r[3] = r0, r1, r2, r3

    return "\n".join(print_region(x) for x in r)  # type: ignore

if __name__ == "__main__":
    data = sys.stdin.read()
    sys.stdout.write(solve(data))
