import sys
import threading
def main():
    import math
    sys.setrecursionlimit(10**7)
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    lines = []
    for _ in range(N):
        x1,y1,x2,y2 = map(int, (next(it),next(it),next(it),next(it)))
        lines.append((x1,y1,x2,y2))

    # Compute intersections exactly as rationals (num,den) in lowest terms
    from fractions import Fraction
    pts_set = set()
    for i in range(N):
        x1,y1,x2,y2 = lines[i]
        dx1, dy1 = x2-x1, y2-y1
        for j in range(i+1, N):
            X1,Y1,X2,Y2 = lines[j]
            dx2, dy2 = X2-X1, Y2-Y1
            det = dx1*dy2 - dy1*dx2
            if det == 0:
                continue  # parallel or coincident
            # solve for parameter t on line i
            num_t = (X1-x1)*dy2 - (Y1-y1)*dx2
            # intersection = (x1 + t*dx1, y1 + t*dy1)
            # as rationals:
            t = Fraction(num_t, det)
            xi = Fraction(x1,1) + t * dx1
            yi = Fraction(y1,1) + t * dy1
            pts_set.add((xi, yi))

    # Index vertices
    verts = list(pts_set)
    V = len(verts)
    vid = {p:i for i,p in enumerate(verts)}
    adj = [[] for _ in range(V)]

    # For each line, collect intersection points on it, sort by parameter t, link consecutive
    for (x1,y1,x2,y2) in lines:
        dx, dy = x2-x1, y2-y1
        temp = []
        for idx,(xi,yi) in enumerate(verts):
            # check collinearity via cross product
            if (xi - x1)*dy == (yi - y1)*dx:
                # project to line: t = (xi-x1)/dx if dx!=0 else (yi-y1)/dy
                if dx != 0:
                    t = (xi - x1)/dx
                else:
                    t = (yi - y1)/dy
                temp.append((t, idx))
        temp.sort(key=lambda x: x[0])
        for k in range(1, len(temp)):
            u = temp[k-1][1]
            v = temp[k][1]
            adj[u].append(v)
            adj[v].append(u)

    # Sort neighbors of each vertex by angle for planar embedding
    nbr_sorted = []
    for i,(xi,yi) in enumerate(verts):
        # remove duplicates
        neigh = sorted(set(adj[i]))
        def angle(j):
            xj,yj = verts[j]
            dx, dy = float(xj-xi), float(yj-yi)
            return math.atan2(dy, dx)
        neigh.sort(key=angle)
        nbr_sorted.append(neigh)

    # Face tracing: each undirected edge → two directed edges
    used = set()
    areas = []
    for u in range(V):
        for v in adj[u]:
            if (u,v) in used:
                continue
            # start face walk from u->v
            su, sv = u, v
            used.add((su,sv))
            cycle = [sv]
            pu, pv = su, sv
            while True:
                lst = nbr_sorted[pv]
                # find index of pu in pv's neighbor list
                k = lst.index(pu)
                # next neighbor in clockwise order = (k-1) mod deg
                nv = lst[(k-1) % len(lst)]
                used.add((pv,nv))
                cycle.append(nv)
                pu, pv = pv, nv
                if pu==su and pv==sv:
                    break
            cycle.pop()  # remove repeated start
            if len(cycle) < 3:
                continue
            # compute signed area
            A = 0
            for i in range(len(cycle)):
                x1,y1 = verts[cycle[i]]
                x2,y2 = verts[cycle[(i+1)%len(cycle)]]
                A += float(x1*y2 - x2*y1)
            A *= 0.5
            if A > 1e-8:
                areas.append(A)

    areas.sort()
    # Output
    print(len(areas))
    for a in areas:
        print(f"{a:.4f}")

if __name__ == "__main__":
    main()
