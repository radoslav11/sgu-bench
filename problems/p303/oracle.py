import sys
import heapq
from math import inf


def main():
    # Read all input tokens at once.
    data = sys.stdin.read().strip().split()

    # If input is empty, do nothing.
    if not data:
        return

    # Pointer into token array.
    ptr = 0

    # Number of border segments.
    n = int(data[ptr])
    ptr += 1

    # Store segment endpoints and costs.
    seg_a = []
    seg_b = []
    seg_cost = []

    for _ in range(n):
        x1 = int(data[ptr])
        y1 = int(data[ptr + 1])
        x2 = int(data[ptr + 2])
        y2 = int(data[ptr + 3])
        v = int(data[ptr + 4])
        ptr += 5

        seg_a.append((x1, y1))
        seg_b.append((x2, y2))
        seg_cost.append(v)

    # Headquarters coordinates.
    hq1 = (int(data[ptr]), int(data[ptr + 1]))
    hq2 = (int(data[ptr + 2]), int(data[ptr + 3]))

    # Map endpoint coordinate pairs to compact vertex ids.
    vertex_id = {}

    def get_id(p):
        """
        Return graph vertex id for point p.
        Create a new id if this endpoint was not seen before.
        """
        if p not in vertex_id:
            vertex_id[p] = len(vertex_id)
        return vertex_id[p]

    def crosses_up_ray(a, b, h):
        """
        Return True if segment ab crosses the upward vertical ray
        starting at point h.

        The ray is:
            x = h.x, y > h.y

        A half-open convention is used to avoid double-counting crossings
        exactly at graph vertices.
        """
        ax, ay = a
        bx, by = b
        hx, hy = h

        # Check which side of vertical line x = hx each endpoint lies on.
        sa = ax > hx
        sb = bx > hx

        # If both are on same side, segment does not cross the vertical line
        # in the required half-open sense.
        if sa == sb:
            return False

        # Compute the segment parameter t at which x = hx.
        # Because sa != sb, bx - ax is nonzero.
        t = (hx - ax) / (bx - ax)

        # y-coordinate of the intersection with x = hx.
        y_cross = ay + t * (by - ay)

        # It crosses the upward ray only if the intersection is above h.
        return y_cross > hy

    # Endpoint ids and parity label for each edge.
    eu = [0] * n
    ev = [0] * n
    epar = [0] * n

    for i in range(n):
        # Convert endpoints into graph vertices.
        eu[i] = get_id(seg_a[i])
        ev[i] = get_id(seg_b[i])

        # Crossing parity for both headquarters.
        c1 = crosses_up_ray(seg_a[i], seg_b[i], hq1)
        c2 = crosses_up_ray(seg_a[i], seg_b[i], hq2)

        # Edge label is 1 iff this edge changes the relative inside/outside
        # parity of the two headquarters.
        epar[i] = int(c1) ^ int(c2)

    # Number of graph vertices.
    m = len(vertex_id)

    # Build undirected adjacency list.
    # adj[v] contains tuples:
    #   (neighbor_vertex, edge_cost, segment_index)
    adj = [[] for _ in range(m)]

    for i in range(n):
        u = eu[i]
        v = ev[i]
        w = seg_cost[i]

        adj[u].append((v, w, i))
        adj[v].append((u, w, i))

    def dijkstra(src_state):
        """
        Dijkstra on doubled graph.

        State encoding:
            state = 2 * vertex + parity

        parity is accumulated XOR of edge labels along the walk.
        """
        total_states = 2 * m

        # Distance array.
        dist = [inf] * total_states

        # Parent array for reconstructing path.
        # parent[state] = (previous_state, segment_index_used)
        parent = [(-1, -1)] * total_states

        # Priority queue of (distance, state).
        pq = []

        # Initialize source.
        dist[src_state] = 0
        heapq.heappush(pq, (0, src_state))

        while pq:
            d, state = heapq.heappop(pq)

            # Ignore stale priority queue entries.
            if d != dist[state]:
                continue

            # Decode current vertex and parity.
            v = state // 2
            parity = state & 1

            # Traverse each original edge.
            for to_vertex, cost, seg_idx in adj[v]:
                # New parity after using this segment.
                new_parity = parity ^ epar[seg_idx]

                # Encode next state.
                next_state = 2 * to_vertex + new_parity

                # Relax edge.
                nd = d + cost
                if nd < dist[next_state]:
                    dist[next_state] = nd
                    parent[next_state] = (state, seg_idx)
                    heapq.heappush(pq, (nd, next_state))

        return dist, parent

    best_cost = inf
    best_src = -1

    # Try every vertex as the beginning/end of the closed walk.
    for s in range(m):
        # Start from vertex s with accumulated parity 0.
        dist, _ = dijkstra(2 * s)

        # We want to return to vertex s with accumulated parity 1.
        target = 2 * s + 1

        if dist[target] < best_cost:
            best_cost = dist[target]
            best_src = s

    # Re-run Dijkstra from best source to reconstruct one optimal cycle.
    dist, parent = dijkstra(2 * best_src)

    # Target state: same vertex, odd parity.
    state = 2 * best_src + 1

    used_segments = []

    # Follow parents backward until source.
    while parent[state][0] != -1:
        prev_state, seg_idx = parent[state]

        # Output uses 1-based segment indices.
        used_segments.append(seg_idx + 1)

        state = prev_state

    # Print answer.
    print(int(best_cost))
    print(len(used_segments))
    print(*used_segments)


if __name__ == "__main__":
    main()
