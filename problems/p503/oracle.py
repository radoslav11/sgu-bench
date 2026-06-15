import sys
import heapq
from collections import deque

INF = 10**30

# We build an Aho–Corasick automaton where "characters" are edge IDs (ints).
class AhoCorasick:
    def __init__(self):
        # next[state] is a dict: symbol -> next_state
        self.nxt = []
        self.link = []
        self._new_state()          # state 0 = root
        self.link[0] = -1          # root suffix link

    def _new_state(self):
        self.nxt.append({})
        self.link.append(0)
        return len(self.nxt) - 1

    def add_word(self, word):
        """Insert a sequence of integers into the trie."""
        u = 0
        for c in word:
            if c not in self.nxt[u]:
                self.nxt[u][c] = self._new_state()
            u = self.nxt[u][c]
        return u  # return terminal state (useful)

    def build(self):
        """Build suffix links using BFS."""
        q = deque()

        # Initialize children of root: suffix link = root
        for c, v in self.nxt[0].items():
            self.link[v] = 0
            q.append(v)

        # BFS
        while q:
            u = q.popleft()
            for c, v in self.nxt[u].items():
                j = self.link[u]
                while j != -1 and c not in self.nxt[j]:
                    j = self.link[j]
                self.link[v] = 0 if j == -1 else self.nxt[j][c]
                q.append(v)

    def go(self, u, c):
        """Automaton transition with failure links."""
        while u != -1 and c not in self.nxt[u]:
            u = self.link[u]
        return 0 if u == -1 else self.nxt[u][c]

    def size(self):
        return len(self.nxt)


def solve():
    data = sys.stdin.buffer.read().split()
    it = iter(data)

    n = int(next(it))
    m = int(next(it))
    r = int(next(it))
    S = int(next(it))
    T = int(next(it))

    edges = [None] * m           # (a,b,c)
    adj = [[] for _ in range(n + 1)]  # adj[a] = (b, edge_id)

    for i in range(m):
        a = int(next(it)); b = int(next(it)); c = int(next(it))
        edges[i] = (a, b, c)
        adj[a].append((b, i))

    routes = []
    for _ in range(r):
        k = int(next(it))
        route = [int(next(it)) - 1 for __ in range(k)]  # 0-based edge IDs
        routes.append(route)

    # --- Build Aho–Corasick ---
    aho = AhoCorasick()

    # pattern_cost[i] = sum of base edge costs along special route i
    pattern_cost = [0] * r

    # We'll need terminal states to assign end_cost easily.
    terminal_states = [0] * r

    for i, route in enumerate(routes):
        cost = 0
        for e in route:
            cost += edges[e][2]
        pattern_cost[i] = cost
        terminal_states[i] = aho.add_word(route)

    # Add singleton patterns {edge_id} with cost 0 (as in C++ solution)
    for e in range(m):
        aho.add_word([e])

    aho.build()
    num_states = aho.size()

    # end_cost[state] = sum of costs of patterns ending exactly at this state
    end_cost = [0] * num_states
    for i in range(r):
        end_cost[terminal_states[i]] += pattern_cost[i]

    # state_cost[state] = sum of end_cost along suffix chain
    state_cost = [0] * num_states
    for u in range(num_states):
        cur = u
        s = 0
        while cur != -1:
            s += end_cost[cur]
            cur = aho.link[cur]
        state_cost[u] = s

    # --- Dijkstra on expanded states (graph_vertex, automaton_state) ---
    dist = [[INF] * num_states for _ in range(n + 1)]
    parent_node = [[-1] * num_states for _ in range(n + 1)]
    parent_state = [[-1] * num_states for _ in range(n + 1)]
    parent_edge = [[-1] * num_states for _ in range(n + 1)]

    pq = []
    dist[S][0] = 0
    heapq.heappush(pq, (0, S, 0))

    while pq:
        d, u, st = heapq.heappop(pq)
        if d != dist[u][st]:
            continue

        for v, edge_id in adj[u]:
            edge_cost = edges[edge_id][2]
            new_st = aho.go(st, edge_id)
            new_dist = d + edge_cost + state_cost[new_st]

            if new_dist < dist[v][new_st]:
                dist[v][new_st] = new_dist
                parent_node[v][new_st] = u
                parent_state[v][new_st] = st
                parent_edge[v][new_st] = edge_id
                heapq.heappush(pq, (new_dist, v, new_st))

    # Pick best automaton state at destination T
    best_dist = INF
    best_st = -1
    for st in range(num_states):
        if dist[T][st] < best_dist:
            best_dist = dist[T][st]
            best_st = st

    if best_dist >= INF:
        sys.stdout.write("-1\n")
        return

    # Reconstruct edge path
    path = []
    cur_node, cur_st = T, best_st
    while parent_edge[cur_node][cur_st] != -1:
        e = parent_edge[cur_node][cur_st]
        path.append(e)
        pn = parent_node[cur_node][cur_st]
        ps = parent_state[cur_node][cur_st]
        cur_node, cur_st = pn, ps
    path.reverse()

    # Output (edge IDs must be 1-based)
    sys.stdout.write(str(best_dist) + "\n")
    sys.stdout.write(str(len(path)) + "\n")
    if path:
        sys.stdout.write(" ".join(str(e + 1) for e in path) + " \n")
    else:
        sys.stdout.write("\n")


if __name__ == "__main__":
    solve()
