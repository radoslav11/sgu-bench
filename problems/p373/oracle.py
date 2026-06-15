import math
from typing import List, Tuple, Optional

EPS = 1e-9
PI = math.pi

class Point:
    __slots__ = ("x", "y")
    def __init__(self, x: float = 0.0, y: float = 0.0):
        self.x = x
        self.y = y

    def __add__(self, other): return Point(self.x + other.x, self.y + other.y)
    def __sub__(self, other): return Point(self.x - other.x, self.y - other.y)
    def __mul__(self, c: float): return Point(self.x * c, self.y * c)
    def __truediv__(self, c: float): return Point(self.x / c, self.y / c)

    def dot(self, other) -> float: return self.x * other.x + self.y * other.y
    def cross(self, other) -> float: return self.x * other.y - self.y * other.x

    def norm2(self) -> float: return self.x * self.x + self.y * self.y
    def norm(self) -> float: return math.hypot(self.x, self.y)

    def perp(self):  # CCW perpendicular
        return Point(-self.y, self.x)

    def unit(self):
        n = self.norm()
        return self / n

def atan2pi(y: float, x: float) -> float:
    """Angle in [0, 2pi)."""
    a = math.atan2(y, x)
    if a < 0:
        a += 2 * PI
    return a

class HalfEdge:
    __slots__ = ("frm", "to", "is_arc", "ccw", "twin", "nxt")
    def __init__(self, frm: int, to: int, is_arc: bool, ccw: bool, twin: int):
        self.frm = frm
        self.to = to
        self.is_arc = is_arc
        self.ccw = ccw
        self.twin = twin
        self.nxt = -1

def solve() -> None:
    import sys
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    r = float(next(it))

    # Read lines as (a,b,c)
    lines: List[Tuple[float, float, float]] = []
    for _ in range(n):
        a = float(next(it)); b = float(next(it)); c = float(next(it))
        lines.append((a, b, c))

    # Compute line-circle intersection points for line i
    def line_circle_pts(i: int) -> Tuple[Point, Point]:
        a, b, c = lines[i]
        n2 = a*a + b*b
        # closest point from origin to line
        foot = Point(-a*c/n2, -b*c/n2)
        # distance^2 along line direction to circle intersections
        h2 = r*r - (c*c)/n2
        if h2 < 0:  # numerical guard; problem guarantees intersection
            h2 = 0.0
        h = math.sqrt(h2)
        # direction along line
        dirv = Point(-b, a) / math.sqrt(n2)
        return foot + dirv*h, foot - dirv*h

    # Compute intersection of lines i and j if not parallel and within disk
    def line_line_pt(i: int, j: int) -> Optional[Point]:
        a1, b1, c1 = lines[i]
        a2, b2, c2 = lines[j]
        d = a1*b2 - a2*b1
        if abs(d) < EPS:
            return None
        x = (-c1*b2 + b1*c2) / d
        y = (-a1*c2 + a2*c1) / d
        p = Point(x, y)
        if p.norm() > r + EPS:
            return None
        return p

    # Vertex list with deduplication
    verts: List[Point] = []

    def add_vertex(p: Point) -> int:
        for idx, q in enumerate(verts):
            if (q - p).norm() < 1e-7:
                return idx
        verts.append(p)
        return len(verts) - 1

    hes: List[HalfEdge] = []

    def add_edge(u: int, v: int, is_arc: bool, ccw_uv: bool) -> None:
        h1 = len(hes)
        # forward
        hes.append(HalfEdge(u, v, is_arc, ccw_uv, h1 + 1))
        # backward (twin)
        hes.append(HalfEdge(v, u, is_arc, not ccw_uv, h1))

    # Add chord segments for each line
    for i in range(n):
        on_line: List[int] = []
        p1, p2 = line_circle_pts(i)
        on_line.append(add_vertex(p1))
        on_line.append(add_vertex(p2))
        for j in range(n):
            if j == i:
                continue
            p = line_line_pt(i, j)
            if p is not None:
                on_line.append(add_vertex(p))

        a, b, _ = lines[i]

        # sort along direction (-b, a) using dot product
        on_line.sort(key=lambda vid: (-b)*verts[vid].x + a*verts[vid].y)

        # unique vertex ids (sorted list)
        uniq = []
        last = None
        for vid in on_line:
            if last is None or vid != last:
                uniq.append(vid)
            last = vid
        on_line = uniq

        # connect consecutive vertices
        for k in range(len(on_line) - 1):
            add_edge(on_line[k], on_line[k+1], is_arc=False, ccw_uv=False)

    # Add boundary arcs between consecutive circle intersection vertices
    circle_verts: List[int] = []
    for i in range(n):
        p1, p2 = line_circle_pts(i)
        circle_verts.append(add_vertex(p1))
        circle_verts.append(add_vertex(p2))

    circle_verts.sort(key=lambda vid: atan2pi(verts[vid].y, verts[vid].x))
    uniq = []
    last = None
    for vid in circle_verts:
        if last is None or vid != last:
            uniq.append(vid)
        last = vid
    circle_verts = uniq

    m = len(circle_verts)
    for k in range(m):
        add_edge(circle_verts[k], circle_verts[(k+1) % m], is_arc=True, ccw_uv=True)

    # Direction used for ordering outgoing half-edges at a vertex
    def outgoing_dir(h: int) -> Point:
        p = verts[hes[h].frm]
        if not hes[h].is_arc:
            return verts[hes[h].to] - p
        # tangent to circle at p; direction depends on arc direction
        if hes[h].ccw:
            return Point(-p.y, p.x)
        else:
            return Point(p.y, -p.x)

    # Build outgoing lists per vertex
    out_at: List[List[int]] = [[] for _ in range(len(verts))]
    for h in range(len(hes)):
        out_at[hes[h].frm].append(h)

    # Sort outgoing edges by polar angle of their direction vector
    for v in range(len(verts)):
        out_at[v].sort(key=lambda h: math.atan2(outgoing_dir(h).y, outgoing_dir(h).x))

    # Position lookup for each half-edge in its vertex outgoing list
    pos_of = [0] * len(hes)
    for v in range(len(verts)):
        for k, h in enumerate(out_at[v]):
            pos_of[h] = k

    # Set next pointers: next(h) = edge clockwise from twin(h) at the destination
    for h in range(len(hes)):
        t = hes[h].twin
        v = hes[t].frm
        k = len(out_at[v])
        hes[h].nxt = out_at[v][(pos_of[t] - 1) % k]

    # Traverse faces and compute positive areas
    visited = [False] * len(hes)
    areas: List[float] = []

    for h0 in range(len(hes)):
        if visited[h0]:
            continue
        area2 = 0.0
        cur = h0
        while not visited[cur]:
            visited[cur] = True
            pf = verts[hes[cur].frm]
            pt = verts[hes[cur].to]

            if not hes[cur].is_arc:
                # segment shoelace contribution
                area2 += pf.x * pt.y - pt.x * pf.y
            else:
                # arc contribution: r^2 * dtheta with sign
                th1 = math.atan2(pf.y, pf.x)
                th2 = math.atan2(pt.y, pt.x)
                dth = th2 - th1
                if hes[cur].ccw:
                    if dth <= 1e-12:
                        dth += 2 * PI
                else:
                    if dth >= -1e-12:
                        dth -= 2 * PI
                area2 += r * r * dth

            cur = hes[cur].nxt

        a = area2 / 2.0
        if a > 1e-9:
            areas.append(a)

    p = len(areas)

    # Alpha-beta minimax returning Carlsson's final area
    sys.setrecursionlimit(10000)

    from functools import lru_cache

    @lru_cache(None)
    def dfs(mask: int, tc_q: int, tp_q: int) -> float:
        """
        Memoized version with quantized tc/tp is hard; instead we do
        non-memo alpha-beta in C++. Here we keep it simple without memo:
        but we still need tc/tp as floats; caching floats is unreliable.
        So we implement a direct alpha-beta recursion without caching below.
        """
        return 0.0  # unused

    def best_carlsson(mask: int, tc: float, tp: float, csf: float,
                      alpha: float, beta: float) -> float:
        if mask == (1 << p) - 1:
            return csf

        c_turn = (tc <= tp + EPS)

        if c_turn:
            best = -1e100
            for i in range(p):
                if mask & (1 << i):
                    continue
                v = best_carlsson(mask | (1 << i),
                                  tc + areas[i], tp,
                                  csf + areas[i],
                                  alpha, beta)
                if v > best:
                    best = v
                if best > alpha:
                    alpha = best
                if alpha >= beta:
                    break
            return best
        else:
            best = 1e100
            for i in range(p):
                if mask & (1 << i):
                    continue
                v = best_carlsson(mask | (1 << i),
                                  tc, tp + areas[i],
                                  csf,
                                  alpha, beta)
                if v < best:
                    best = v
                if best < beta:
                    beta = best
                if alpha >= beta:
                    break
            return best

    total = sum(areas)
    carl = best_carlsson(0, 0.0, 0.0, 0.0, -1e100, 1e100)
    pooh = total - carl

    sys.stdout.write(f"{carl:.10f} {pooh:.10f}\n")

if __name__ == "__main__":
    solve()
