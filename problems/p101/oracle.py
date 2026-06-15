import sys
sys.setrecursionlimit(10**7)

def read_input():
    """Read N and the list of dominoes."""
    N = int(sys.stdin.readline().strip())
    dominos = [tuple(map(int, sys.stdin.readline().split())) for _ in range(N)]
    return N, dominos

class Eulerian:
    def __init__(self, max_vertex):
        self.n = max_vertex
        self.adj = [[] for _ in range(self.n+1)]
        self.edges = []     # list of (u,v)
        self.deg = [0]*(self.n+1)
        self.m = 0

    def add_edge(self, u, v):
        """Add an undirected edge as two half-edges with IDs 2*m, 2*m+1."""
        heid1 = 2*self.m
        heid2 = 2*self.m+1
        self.adj[u].append((v, heid1))
        self.adj[v].append((u, heid2))
        self.edges.append((u, v))
        self.deg[u] += 1
        self.deg[v] += 1
        self.m += 1

    def find_trails(self):
        """Pair odd vertices, run Hierholzer, and extract trails."""
        used = [False]*self.m
        # Find odd-degree vertices
        odd = [v for v in range(self.n+1) if self.deg[v] % 2 == 1]
        # If >2 odd vertices, no solution
        if len(odd) > 2:
            return None

        # Pair odd vertices (if 2) by adding a fake edge
        tot = self.m
        if len(odd) == 2:
            u, v = odd
            # Fake half-edges IDs: 2*tot, 2*tot+1
            self.adj[u].append((v, 2*tot))
            self.adj[v].append((u, 2*tot+1))
            self.edges.append((u, v))
            used.append(False)
            tot += 1

        # Prepare for Hierholzer
        ptr = [0]*(self.n+1)
        trails = []

        def dfs(u, path):
            """Recursively follow unused halves to build a cycle."""
            while ptr[u] < len(self.adj[u]):
                v, heid = self.adj[u][ptr[u]]
                ptr[u] += 1
                idx = heid >> 1
                if not used[idx]:
                    used[idx] = True
                    dfs(v, path)
                    path.append(heid)

        # Run DFS from every vertex that has edges
        for start in range(self.n+1):
            if ptr[start] < len(self.adj[start]):
                cycle = []
                dfs(start, cycle)
                if not cycle:
                    continue
                # If we added a fake edge, rotate so fake is first
                fake_id = 2*self.m
                for i, he in enumerate(cycle):
                    if he >= fake_id:
                        cycle = cycle[i:] + cycle[:i]
                        break
                # Split at fake edges (if any) to form actual trails
                cur = []
                for he in cycle:
                    if he < 2*self.m:
                        cur.append(he)
                    else:
                        if cur:
                            trails.append(cur)
                            cur = []
                if cur:
                    trails.append(cur)

        # We need exactly one trail using all original edges
        if len(trails) == 1 and len(trails[0]) == self.m:
            return trails[0]
        else:
            return None

    def oriented_edge(self, heid):
        """Given half-edge ID, return oriented (u,v)."""
        idx = heid >> 1
        u, v = self.edges[idx]
        # if heid is even, direction is u->v; if odd, v->u
        if heid & 1:
            return (v, u)
        else:
            return (u, v)


def main():
    N, dominos = read_input()
    E = Eulerian(6)
    for u, v in dominos:
        E.add_edge(u, v)

    trail = E.find_trails()
    if trail is None:
        print("No solution")
        return

    # Output each edge's index and orientation. dfs appends half-edges in
    # post-order, so the collected trail is the reverse of the traversal;
    # iterate it backwards to get a left-to-right chain.
    for heid in reversed(trail):
        idx = (heid >> 1) + 1          # 1-based domino index
        u,v = E.oriented_edge(heid)
        # Check if this matches the original orientation
        if (u, v) == dominos[heid >> 1]:
            print(idx, '+')
        else:
            print(idx, '-')

if __name__ == "__main__":
    main()
