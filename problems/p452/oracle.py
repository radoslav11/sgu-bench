import sys
import math
import heapq


EPS = 1e-9
INF = 10**100


# ------------------------------------------------------------
# 2D point class used for unfolded faces.
# ------------------------------------------------------------
class Point:
    __slots__ = ("x", "y")

    def __init__(self, x=0.0, y=0.0):
        self.x = float(x)
        self.y = float(y)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, value):
        # Multiplication by scalar.
        return Point(self.x * value, self.y * value)

    def __truediv__(self, value):
        return Point(self.x / value, self.y / value)

    def norm2(self):
        return self.x * self.x + self.y * self.y

    def norm(self):
        return math.sqrt(self.norm2())

    def perp(self):
        return Point(-self.y, self.x)


def dot2(a, b):
    return a.x * b.x + a.y * b.y


def cross2(a, b):
    return a.x * b.y - a.y * b.x


def sub3(a, b):
    return (a[0] - b[0], a[1] - b[1], a[2] - b[2])


def dot3(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]


# Six axis directions.
DVEC = [
    (1, 0, 0),
    (-1, 0, 0),
    (0, 1, 0),
    (0, -1, 0),
    (0, 0, 1),
    (0, 0, -1),
]


def opp(d):
    return d ^ 1


# Canonical 2D square.
CANON = [
    Point(0, 0),
    Point(100, 0),
    Point(100, 100),
    Point(0, 100),
]


class DSU:
    def __init__(self, n):
        self.parent = list(range(n))

    def find(self, x):
        while self.parent[x] != x:
            self.parent[x] = self.parent[self.parent[x]]
            x = self.parent[x]
        return x

    def union(self, a, b):
        ra = self.find(a)
        rb = self.find(b)
        if ra != rb:
            self.parent[ra] = rb


class Solver:
    def __init__(self, centers, src_pt, dst_pt):
        self.centers = centers
        self.src_pt = src_pt
        self.dst_pt = dst_pt

        self.grid_set = set()

        self.face_center = []
        self.face_dir = []
        self.face_grid = []
        self.face_corner = []
        self.face_id = {}

        self.nb_face = []
        self.nb_entry = []

        self.sheet_of = []
        self.num_sheets = 0

        self.face_of_src = -1
        self.face_of_dst = -1

        self.ub = INF

    def present(self, gx, gy, gz):
        return (gx, gy, gz) in self.grid_set

    # ------------------------------------------------------------
    # Build all exposed faces.
    # ------------------------------------------------------------
    def build_faces(self):
        for c in self.centers:
            self.grid_set.add((c[0] // 100, c[1] // 100, c[2] // 100))

        for c in self.centers:
            gx = c[0] // 100
            gy = c[1] // 100
            gz = c[2] // 100

            for d in range(6):
                nx = gx + DVEC[d][0]
                ny = gy + DVEC[d][1]
                nz = gz + DVEC[d][2]

                # Hidden face.
                if self.present(nx, ny, nz):
                    continue

                # Face center.
                fc = (
                    c[0] + 50 * DVEC[d][0],
                    c[1] + 50 * DVEC[d][1],
                    c[2] + 50 * DVEC[d][2],
                )

                axis = d // 2

                # The two axes lying in this face.
                axes = [a for a in range(3) if a != axis]
                la, lb = axes[0], axes[1]

                signs = [(-1, -1), (1, -1), (1, 1), (-1, 1)]

                corners = []

                for sa, sb in signs:
                    p = [fc[0], fc[1], fc[2]]
                    p[la] += 50 * sa
                    p[lb] += 50 * sb
                    corners.append(tuple(p))

                fid = len(self.face_center)

                self.face_id[(gx, gy, gz, d)] = fid
                self.face_center.append(fc)
                self.face_dir.append(d)
                self.face_grid.append((gx, gy, gz))
                self.face_corner.append(corners)

    # ------------------------------------------------------------
    # Build surface adjacency across edges.
    # ------------------------------------------------------------
    def build_adjacency(self):
        nf = len(self.face_center)

        self.nb_face = [[-1] * 4 for _ in range(nf)]
        self.nb_entry = [[-1] * 4 for _ in range(nf)]

        for f in range(nf):
            d = self.face_dir[f]
            g = self.face_grid[f]

            for e in range(4):
                a = self.face_corner[f][e]
                b = self.face_corner[f][(e + 1) % 4]

                mid = (
                    (a[0] + b[0]) // 2,
                    (a[1] + b[1]) // 2,
                    (a[2] + b[2]) // 2,
                )

                delta = sub3(mid, self.face_center[f])

                t = -1

                for dd in range(6):
                    if delta == (
                        50 * DVEC[dd][0],
                        50 * DVEC[dd][1],
                        50 * DVEC[dd][2],
                    ):
                        t = dd
                        break

                ax = g[0] + DVEC[t][0]
                ay = g[1] + DVEC[t][1]
                az = g[2] + DVEC[t][2]

                cx = ax + DVEC[d][0]
                cy = ay + DVEC[d][1]
                cz = az + DVEC[d][2]

                if not self.present(ax, ay, az):
                    key = (g[0], g[1], g[2], t)
                elif not self.present(cx, cy, cz):
                    key = (ax, ay, az, d)
                else:
                    key = (cx, cy, cz, opp(t))

                gface = self.face_id[key]

                self.nb_face[f][e] = gface

                found = -1

                for k in range(4):
                    p = self.face_corner[gface][k]
                    q = self.face_corner[gface][(k + 1) % 4]

                    if (p == a and q == b) or (p == b and q == a):
                        found = k
                        break

                self.nb_entry[f][e] = found

    def corner_in(self, face, vertex):
        for k in range(4):
            if self.face_corner[face][k] == vertex:
                return k
        return -1

    # ------------------------------------------------------------
    # Build vertex sheets.
    # ------------------------------------------------------------
    def build_sheets(self):
        nf = len(self.face_center)

        dsu = DSU(nf * 4)

        for f in range(nf):
            for k in range(4):
                vertex = self.face_corner[f][k]

                # Two incident edges at this face corner.
                for e in (k, (k + 3) % 4):
                    g = self.nb_face[f][e]
                    kk = self.corner_in(g, vertex)

                    dsu.union(f * 4 + k, g * 4 + kk)

        remap = {}
        self.sheet_of = [-1] * (nf * 4)
        self.num_sheets = 0

        for i in range(nf * 4):
            r = dsu.find(i)

            if r not in remap:
                remap[r] = self.num_sheets
                self.num_sheets += 1

            self.sheet_of[i] = remap[r]

    # ------------------------------------------------------------
    # Find exposed face containing a given point.
    # ------------------------------------------------------------
    def find_face_of(self, p):
        for f, fc in enumerate(self.face_center):
            d = self.face_dir[f]
            axis = d // 2

            if p[axis] != fc[axis]:
                continue

            ok = True

            for a in range(3):
                if a == axis:
                    continue

                # Input guarantees the point is not on an edge.
                if abs(p[a] - fc[a]) >= 50:
                    ok = False
                    break

            if ok:
                return f

        return -1

    # ------------------------------------------------------------
    # Convert a 3D point on a face into the current unfolded 2D frame.
    # ------------------------------------------------------------
    def to_frame(self, p, f, c2):
        o = self.face_corner[f][0]

        edge01 = sub3(self.face_corner[f][1], o)
        edge03 = sub3(self.face_corner[f][3], o)

        po = sub3(p, o)

        alpha = dot3(po, edge01) / 10000.0
        beta = dot3(po, edge03) / 10000.0

        return c2[0] + (c2[1] - c2[0]) * alpha + (c2[3] - c2[0]) * beta

    def in_sector(self, apex, lo, hi, pt):
        dl = lo - apex
        dr = hi - apex

        if cross2(dl, dr) < 0:
            dl, dr = dr, dl

        v = pt - apex

        tol = 1e-9 * v.norm() + 1e-12

        tl = tol * dl.norm()
        tr = tol * dr.norm()

        return cross2(dl, v) >= -tl and cross2(dr, v) <= tr

    def clip_half(self, a, b, s0, s1):
        if abs(b) < 1e-15:
            if a < -1e-12:
                return 1.0, 0.0
            return s0, s1

        r = -a / b

        if b > 0:
            s0 = max(s0, r)
        else:
            s1 = min(s1, r)

        return s0, s1

    # ------------------------------------------------------------
    # Clip segment cd to angular sector.
    # ------------------------------------------------------------
    def clip_sector(self, apex, lo, hi, c, d):
        dl = lo - apex
        dr = hi - apex

        if cross2(dl, dr) < 0:
            dl, dr = dr, dl

        s0 = 0.0
        s1 = 1.0

        dc = d - c

        s0, s1 = self.clip_half(cross2(dl, c - apex), cross2(dl, dc), s0, s1)
        s0, s1 = self.clip_half(-cross2(dr, c - apex), -cross2(dr, dc), s0, s1)

        if s0 > s1 - 1e-12:
            return None

        q0 = c + dc * s0
        q1 = c + dc * s1

        return q0, q1

    def point_seg_dist(self, p, a, b):
        ab = b - a

        length2 = dot2(ab, ab)

        if length2 < 1e-18:
            return (p - a).norm()

        t = dot2(p - a, ab) / length2
        t = max(0.0, min(1.0, t))

        proj = a + ab * t

        return (p - proj).norm()

    # ------------------------------------------------------------
    # Place a far corner of the neighboring face in the unfolded plane.
    # ------------------------------------------------------------
    def place_corner(self, p0, p1, q, m0, m1, apex):
        along = dot3(sub3(q, p0), sub3(p1, p0)) / 100.0

        dsq = dot3(sub3(q, p0), sub3(q, p0))

        h = math.sqrt(max(0.0, dsq - along * along))

        u = (m1 - m0) * (1.0 / 100.0)

        base = m0 + u * along

        cand1 = base + u.perp() * h
        cand2 = base - u.perp() * h

        side = cross2(m1 - m0, apex - m0)
        s1 = cross2(m1 - m0, cand1 - m0)

        if (side >= 0) == (s1 >= 0):
            return cand2

        return cand1

    # ------------------------------------------------------------
    # Coarse upper bound using face-center graph.
    # ------------------------------------------------------------
    def compute_ub(self):
        nf = len(self.face_center)

        total = nf + 2

        graph = [[] for _ in range(total)]

        for f in range(nf):
            for e in range(4):
                graph[f].append((self.nb_face[f][e], 100.0))

        def dist3(a, b):
            d = sub3(a, b)
            return math.sqrt(dot3(d, d))

        src_node = nf
        dst_node = nf + 1

        ws = dist3(self.src_pt, self.face_center[self.face_of_src])
        wd = dist3(self.dst_pt, self.face_center[self.face_of_dst])

        graph[src_node].append((self.face_of_src, ws))
        graph[self.face_of_src].append((src_node, ws))

        graph[dst_node].append((self.face_of_dst, wd))
        graph[self.face_of_dst].append((dst_node, wd))

        dist = [INF] * total
        dist[src_node] = 0.0

        pq = [(0.0, src_node)]

        while pq:
            cur, u = heapq.heappop(pq)

            if cur > dist[u] + 1e-9:
                continue

            for v, w in graph[u]:
                nd = cur + w

                if nd + 1e-9 < dist[v]:
                    dist[v] = nd
                    heapq.heappush(pq, (nd, v))

        self.ub = dist[dst_node]

    # ------------------------------------------------------------
    # Run one unfolding sweep from a source graph node.
    # ------------------------------------------------------------
    def run_field(self, starts, source_node):
        total = self.num_sheets + 2

        dist = [INF] * total
        dist[source_node] = 0.0

        stack = []

        for face, apex in starts:
            c2 = [Point(p.x, p.y) for p in CANON]

            stack.append((face, -1, 0, apex, Point(0, 0), Point(0, 0), c2))

        while stack:
            face, entry, depth, apex, lo, hi, c2 = stack.pop()

            # Record visible corners.
            for k in range(4):
                pt = c2[k]

                if entry == -1 or self.in_sector(apex, lo, hi, pt):
                    node = self.sheet_of[face * 4 + k]
                    dd = (pt - apex).norm()

                    if dd < dist[node]:
                        dist[node] = dd

            # Record source point if visible.
            if self.face_of_src == face:
                sp = self.to_frame(self.src_pt, face, c2)

                if entry == -1 or self.in_sector(apex, lo, hi, sp):
                    dd = (sp - apex).norm()

                    if dd < dist[self.num_sheets]:
                        dist[self.num_sheets] = dd

            # Record destination point if visible.
            if self.face_of_dst == face:
                dp = self.to_frame(self.dst_pt, face, c2)

                if entry == -1 or self.in_sector(apex, lo, hi, dp):
                    dd = (dp - apex).norm()

                    if dd < dist[self.num_sheets + 1]:
                        dist[self.num_sheets + 1] = dd

            # Propagate through all edges except the entry edge.
            for e in range(4):
                if e == entry:
                    continue

                c = c2[e]
                d = c2[(e + 1) % 4]

                if entry == -1:
                    # Avoid degenerate windows starting exactly along an incident edge.
                    if (c - apex).norm() < 1e-9 or (d - apex).norm() < 1e-9:
                        continue

                    q0, q1 = c, d
                else:
                    clipped = self.clip_sector(apex, lo, hi, c, d)

                    if clipped is None:
                        continue

                    q0, q1 = clipped

                # If this window is already farther than known answer, discard.
                if self.point_seg_dist(apex, q0, q1) > self.ub + 1e-6:
                    continue

                if depth + 1 > 256:
                    continue

                g = self.nb_face[face][e]
                eg = self.nb_entry[face][e]

                p0 = self.face_corner[face][e]
                p1 = self.face_corner[face][(e + 1) % 4]

                m0 = c
                m1 = d

                c2g = [None] * 4

                # Place neighboring face corners.
                for k in range(4):
                    cc = self.face_corner[g][k]

                    if cc == p0:
                        c2g[k] = m0
                    elif cc == p1:
                        c2g[k] = m1
                    else:
                        c2g[k] = self.place_corner(p0, p1, cc, m0, m1, apex)

                stack.append((g, eg, depth + 1, apex, q0, q1, c2g))

        return dist

    # ------------------------------------------------------------
    # Full solve routine.
    # ------------------------------------------------------------
    def solve(self):
        self.build_faces()
        self.build_adjacency()
        self.build_sheets()

        self.face_of_src = self.find_face_of(self.src_pt)
        self.face_of_dst = self.find_face_of(self.dst_pt)

        self.compute_ub()

        nf = len(self.face_center)

        incident = [[] for _ in range(self.num_sheets)]

        for f in range(nf):
            for k in range(4):
                sheet = self.sheet_of[f * 4 + k]
                incident[sheet].append((f, k))

        total = self.num_sheets + 2

        src_node = self.num_sheets
        dst_node = self.num_sheets + 1

        field = [None] * total

        # Field sweeps from vertex sheets.
        for v in range(self.num_sheets):
            starts = []

            for f, k in incident[v]:
                starts.append((f, Point(CANON[k].x, CANON[k].y)))

            field[v] = self.run_field(starts, v)

        # Field sweep from source point.
        c2 = [Point(p.x, p.y) for p in CANON]
        sp = self.to_frame(self.src_pt, self.face_of_src, c2)

        field[src_node] = self.run_field([(self.face_of_src, sp)], src_node)

        # Dijkstra over graph nodes.
        dist = [INF] * total
        dist[src_node] = 0.0

        pq = [(0.0, src_node)]

        while pq:
            cur, u = heapq.heappop(pq)

            if cur > dist[u] + 1e-12:
                continue

            if u == dst_node:
                continue

            row = field[u]

            for v in range(total):
                w = row[v]

                if w > 1e90:
                    continue

                nd = cur + w

                if nd + 1e-12 < dist[v]:
                    dist[v] = nd
                    heapq.heappush(pq, (nd, v))

        return dist[dst_node]


def main():
    data = list(map(int, sys.stdin.read().split()))

    if not data:
        return

    idx = 0

    n = data[idx]
    idx += 1

    centers = []

    for _ in range(n):
        x, y, z = data[idx], data[idx + 1], data[idx + 2]
        idx += 3
        centers.append((x, y, z))

    sx, sy, sz, dx, dy, dz = data[idx:idx + 6]

    src_pt = (sx, sy, sz)
    dst_pt = (dx, dy, dz)

    solver = Solver(centers, src_pt, dst_pt)

    ans = solver.solve()

    print(f"{ans:.10f}")


if __name__ == "__main__":
    main()
