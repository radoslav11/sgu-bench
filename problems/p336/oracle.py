import sys

def solve() -> None:
    sys.setrecursionlimit(1_000_000)
    input = sys.stdin.readline

    n, m = map(int, input().split())

    # know[u] = outgoing adjacency list (representatives are 0..n-1 only)
    know = [[] for _ in range(n)]
    for _ in range(m):
        a, b = map(int, input().split())
        a -= 1; b -= 1
        know[a].append(b)

    q = int(input())

    # id maps external entity index -> internal representative
    # external indices start with 0..n-1 and we append for each merge
    id_map = list(range(n))

    # active reps: only original reps exist, some will be "killed" by merges
    active = [True] * n

    # appears[v] = list of sources u such that u -> v exists (reverse edges)
    appears = [[] for _ in range(n)]
    for u in range(n):
        for v in know[u]:
            appears[v].append(u)

    # We need fast edge existence queries between representatives.
    # Representants are in [0..n-1], so we can encode (u,v) as u*MOD + v
    MOD = n + 1
    edges = set()
    for u in range(n):
        for v in know[u]:
            edges.add(u * MOD + v)

    out_lines = []

    for _ in range(q):
        t, a, b = map(int, input().split())
        a -= 1; b -= 1

        if t == 1:
            ra = id_map[a]
            rb = id_map[b]
            out_lines.append("YES" if (ra * MOD + rb) in edges else "NO")
        else:
            u = id_map[a]
            v = id_map[b]
            if u == v:
                # still must create a new block in statement?
                # Actually queries guarantee a!=b for type 2; but could map to same rep.
                # In that case, no structural change, but a new external id would still exist.
                # The C++ code 'continue's (does not push new id).
                # We mimic C++ exactly:
                continue

            # Small-to-large heuristic: keep u as larger
            if len(know[u]) + len(appears[u]) < len(know[v]) + len(appears[v]):
                u, v = v, u

            # Move outgoing edges v -> x into u -> x (with v remapped to u)
            for x in know[v]:
                if not active[x]:
                    continue
                target = u if x == v else x
                know[u].append(target)
                appears[target].append(u)
                edges.add(u * MOD + target)

            # Move incoming edges x -> v into x -> u (with v remapped to u)
            for x in appears[v]:
                if not active[x]:
                    continue
                source = u if x == v else x
                know[source].append(u)
                appears[u].append(source)
                edges.add(source * MOD + u)

            # Kill v
            know[v].clear()
            appears[v].clear()
            active[v] = False

            # New external block id points to representative u
            id_map.append(u)

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
