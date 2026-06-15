import sys
from collections import deque, defaultdict


def main():
    # Read all integers from input.
    data = list(map(int, sys.stdin.buffer.read().split()))

    # First two integers are n and m.
    n = data[0]
    m = data[1]

    # Build adjacency list.
    adj = [[] for _ in range(n + 1)]

    idx = 2
    for _ in range(m):
        a = data[idx]
        b = data[idx + 1]
        idx += 2

        adj[a].append(b)
        adj[b].append(a)

    # Compute degree of each vertex.
    deg = [0] * (n + 1)
    for v in range(1, n + 1):
        deg[v] = len(adj[v])

    # arcs will store triples:
    # (junction1, junction2, number_of_internal_degree_2_vertices)
    arcs = []

    # To avoid discovering the same arc twice, remember directed first edges.
    seen_first_edge = set()

    # Start from every junction.
    for j in range(1, n + 1):
        # Junctions are exactly degree-4 vertices.
        if deg[j] != 4:
            continue

        # Each neighbor starts one possible arc.
        for x in adj[j]:
            # If this directed boundary edge was already processed, skip it.
            if (j, x) in seen_first_edge:
                continue

            # Walk from junction j through degree-2 vertices.
            interior_count = 0
            prev = j
            cur = x

            # Continue until another degree-4 junction is reached.
            while deg[cur] == 2:
                interior_count += 1

                # Move to the neighbor different from prev.
                if adj[cur][0] == prev:
                    nxt = adj[cur][1]
                else:
                    nxt = adj[cur][0]

                prev, cur = cur, nxt

            # Now cur is the other junction.
            # Mark both directions of this arc as seen.
            seen_first_edge.add((j, x))
            seen_first_edge.add((cur, prev))

            # Store the arc.
            arcs.append((j, cur, interior_count))

    # Group arcs by unordered pair of endpoint junctions.
    by_pair = defaultdict(list)

    for i, (j1, j2, cnt) in enumerate(arcs):
        a = min(j1, j2)
        b = max(j1, j2)
        by_pair[(a, b)].append(i)

    # Reconstruct all areas.
    #
    # Each area is represented as a dictionary:
    # {
    #   "j1": first junction,
    #   "j2": second junction,
    #   "size": number of vertices in area cycle,
    #   "odd": whether size is odd,
    #   "arc_parity": parity of one arc edge-length
    # }
    areas = []

    for (j1, j2), ids in by_pair.items():
        # There are exactly two arcs between the same pair of junctions.
        id1 = ids[0]
        id2 = ids[1]

        len1 = arcs[id1][2]  # Internal vertices on first arc.
        len2 = arcs[id2][2]  # Internal vertices on second arc.

        # Area vertices are two junctions plus internal vertices of both arcs.
        size = 2 + len1 + len2

        # Edge-length parity of first arc is len1 + 1.
        arc_parity = (len1 + 1) % 2

        areas.append({
            "j1": j1,
            "j2": j2,
            "size": size,
            "odd": size % 2 == 1,
            "arc_parity": arc_parity,
        })

    k = len(areas)

    # For every junction, store the two areas that contain it.
    jun_areas = defaultdict(list)

    for i, ar in enumerate(areas):
        jun_areas[ar["j1"]].append(i)
        jun_areas[ar["j2"]].append(i)

    # Order the areas around the meta-cycle.
    area_order = [0]

    cur_area = 0

    # Choose one junction of area 0 to define traversal direction.
    next_junction = areas[0]["j2"]

    # Walk through neighboring areas until all are ordered.
    while len(area_order) < k:
        shared_areas = jun_areas[next_junction]

        # Move to the other area sharing next_junction.
        if shared_areas[0] == cur_area:
            next_area = shared_areas[1]
        else:
            next_area = shared_areas[0]

        area_order.append(next_area)

        # Leave next_area through its other junction.
        prev_junction = next_junction

        if areas[next_area]["j1"] == prev_junction:
            next_junction = areas[next_area]["j2"]
        else:
            next_junction = areas[next_area]["j1"]

        cur_area = next_area

    # ordered_junctions[i] is the junction between area_order[i]
    # and area_order[(i + 1) % k].
    ordered_junctions = [0] * k

    for i in range(k):
        a = area_order[i]
        b = area_order[(i + 1) % k]

        # Find the common junction.
        if areas[a]["j1"] == areas[b]["j1"] or areas[a]["j1"] == areas[b]["j2"]:
            ordered_junctions[i] = areas[a]["j1"]
        else:
            ordered_junctions[i] = areas[a]["j2"]

    # Boolean array telling whether each ordered area is odd.
    is_odd = [False] * k

    for i in range(k):
        is_odd[i] = areas[area_order[i]]["odd"]

    # Count odd areas.
    odd_count = sum(is_odd)

    # remove_junction[i] means deleting ordered_junctions[i],
    # the junction between ordered area i and i+1.
    remove_junction = [False] * k

    if odd_count == 0:
        # All area cycles are even.
        # Check the possible global cycle parity.
        macro_parity = 0

        for i in range(k):
            macro_parity ^= areas[area_order[i]]["arc_parity"]

        # If the global cycle is odd, delete any junction.
        if macro_parity != 0:
            remove_junction[0] = True

    elif odd_count == k:
        # Every area is odd.
        # Pair consecutive areas greedily.
        i = 0
        while i + 1 < k:
            remove_junction[i] = True
            i += 2

        # If one odd area remains, delete the wrap-around junction.
        if k % 2 == 1:
            remove_junction[k - 1] = True

    else:
        # There is at least one even area.
        # Start from an even area so odd areas appear as linear runs.
        start_pos = 0

        while is_odd[start_pos]:
            start_pos = (start_pos + 1) % k

        i = 0

        while i < k:
            pos = (start_pos + i) % k

            # Skip even areas.
            if not is_odd[pos]:
                i += 1
                continue

            # Measure this run of consecutive odd areas.
            run_len = 0

            while i + run_len < k:
                cur_pos = (start_pos + i + run_len) % k
                if not is_odd[cur_pos]:
                    break
                run_len += 1

            # Pair odd areas inside the run.
            j = 0
            while j + 1 < run_len:
                p1 = (start_pos + i + j) % k

                # Delete the junction between p1 and p1+1.
                remove_junction[p1] = True

                j += 2

            # If the run length is odd, cover the leftover odd area.
            if run_len % 2 == 1:
                last_pos = (start_pos + i + run_len - 1) % k
                remove_junction[last_pos] = True

            i += run_len

    # Convert selected junction positions into actual removed vertices.
    removed = set()

    for i in range(k):
        if remove_junction[i]:
            removed.add(ordered_junctions[i])

    # Bipartite-color the remaining graph.
    color = [-1] * (n + 1)

    for s in range(1, n + 1):
        # Skip deleted and already colored vertices.
        if s in removed or color[s] != -1:
            continue

        # BFS from this component.
        q = deque([s])
        color[s] = 0

        while q:
            v = q.popleft()

            for u in adj[v]:
                # Removed vertices do not exist in the remaining graph.
                if u in removed:
                    continue

                if color[u] == -1:
                    color[u] = 1 - color[v]
                    q.append(u)

    # Collect both color classes.
    c0 = []
    c1 = []

    for v in range(1, n + 1):
        if color[v] == 0:
            c0.append(v)
        elif color[v] == 1:
            c1.append(v)

    # Print answer.
    out = []

    out.append(str(len(c0)) + (" " + " ".join(map(str, c0)) if c0 else ""))
    out.append(str(len(c1)) + (" " + " ".join(map(str, c1)) if c1 else ""))

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
