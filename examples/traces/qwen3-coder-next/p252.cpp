#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <climits>

using namespace std;

struct Edge {
    int to, cost, rev;
    long long cap;
    Edge(int t, int c, int r, long long cp) : to(t), cost(c), rev(r), cap(cp) {}
};

struct MinCostFlow {
    int n;
    vector<vector<Edge>> g;
    vector<long long> dist, potential;
    vector<int> prevv, preve;

    MinCostFlow(int nodes) : n(nodes), g(nodes), dist(nodes), potential(nodes, 0), 
                             prevv(nodes), preve(nodes) {}

    void addEdge(int from, int to, int cap, int cost) {
        g[from].emplace_back(to, cost, (int)g[to].size(), cap);
        g[to].emplace_back(from, -cost, (int)g[from].size() - 1, 0);
    }

    pair<long long, long long> minCostFlow(int s, int t, int f) {
        long long flow = 0, cost = 0;
        while (f > 0) {
            vector<long long> dist(n, LLONG_MAX);
            dist[s] = 0;
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
            pq.push({0, s});
            fill(prevv.begin(), prevv.end(), -1);
            fill(preve.begin(), preve.end(), -1);

            while (!pq.empty()) {
                auto [d, v] = pq.top(); pq.pop();
                if (dist[v] < d) continue;
                for (int i = 0; i < (int)g[v].size(); ++i) {
                    auto& e = g[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + potential[v] - potential[e.to]) {
                        dist[e.to] = dist[v] + e.cost + potential[v] - potential[e.to];
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[t] == LLONG_MAX) return {flow, cost};
            for (int v = 0; v < n; ++v) {
                if (dist[v] < LLONG_MAX)
                    potential[v] += dist[v];
            }

            long long d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = min(d, g[prevv[v]][preve[v]].cap);
            }
            f -= d;
            flow += d;
            cost += d * potential[t];

            for (int v = t; v != s; v = prevv[v]) {
                auto& e = g[prevv[v]][preve[v]];
                e.cap -= d;
                g[v][e.rev].cap += d;
            }
        }
        return {flow, cost};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    // Build flow network for minimum path cover in DAG
    // Each town i is split into in-node i and out-node i+N
    // Source connects to all out-nodes, in-nodes connect to sink
    // Original edges go from out-node to in-node

    MinCostFlow mcf(2 * N + 2);
    int source = 0, sink = 2 * N + 1;

    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        // Out-node of a (a+N) -> In-node of b (b)
        mcf.addEdge(a + N, b, 1, c);
    }

    // Connect source to all out-nodes (capacity 1, cost 0)
    for (int i = 0; i < N; ++i) {
        mcf.addEdge(source, i + N, 1, 0);
    }

    // Connect all in-nodes to sink (capacity 1, cost 0)
    for (int i = 0; i < N; ++i) {
        mcf.addEdge(i, sink, 1, 0);
    }

    auto [flow, totalCost] = mcf.minCostFlow(source, sink, N);

    // We need exactly N units of flow to cover all towns
    // But the minimal path cover is N - flow, and we must cover all towns
    // Actually, in our construction, each vertex can be used only once (capacity 1 on in/out edges)
    // So the flow will be N (since each node must be covered exactly once)
    // But wait, condition 3 says each town must be covered by at least one path, and condition 2 says at most once.
    // So each town must be in exactly one path.

    // Number of paths = N - flow (standard DAG path cover)
    // But in our setup, flow is the number of edges in the paths, so paths = N - flow.
    // But we required all towns covered, so flow must be N - (number of paths).
    // Since each path of length L uses L-1 edges, total edges = N - paths.

    // Actually, our flow network is set up so that each node can have at most one incoming and one outgoing edge in the path cover.
    // So the flow value is the number of edges used, and number of paths = N - flow_value.

    // But note: in our network, we forced exactly one unit into each in-node and one out of each out-node,
    // so flow should be exactly N? Let me reconsider.

    // Correction: In our network, each in-node (0..N-1) has capacity 1 to sink, so at most one unit enters each in-node.
    // Each out-node (N..2N-1) has capacity 1 from source, so at most one unit leaves each out-node.
    // A path uses one out-node and one in-node per vertex, but the same vertex's in and out nodes are connected implicitly.
    // Actually, we didn't add edges between in-node and out-node of the same vertex! That's a problem.

    // Standard vertex-capacitated path cover in DAG: 
    // Split each vertex v into v_in and v_out.
    // Add edge v_in -> v_out with capacity 1, cost 0.
    // For each original edge u->v, add edge u_out -> v_in with capacity 1, cost = original cost.
    // Source -> v_out (cap=1, cost=0), v_in -> sink (cap=1, cost=0)

    // Let me rebuild:

    MinCostFlow mcf2(2 * N + 2);
    int s = 0, t = 2 * N + 1;

    // Internal edges for each vertex: in-node -> out-node
    for (int i = 0; i < N; ++i) {
        mcf2.addEdge(i, i + N, 1, 0); // in-node i to out-node i+N
    }

    // Source to each out-node
    for (int i = 0; i < N; ++i) {
        mcf2.addEdge(s, i + N, 1, 0);
    }

    // Each in-node to sink
    for (int i = 0; i < N; ++i) {
        mcf2.addEdge(i, t, 1, 0);
    }

    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        // out-node of a (a+N) -> in-node of b (b)
        mcf2.addEdge(a + N, b, 1, c);
    }

    auto [flow2, totalCost2] = mcf2.minCostFlow(s, t, N);

    // The flow is N (since we forced N units, and the graph ensures a feasible cover exists: each vertex must be covered once)
    // Number of paths = N - flow2 = N - N = 0? That's wrong.

    // Actually, in this network, the minimum cost flow of value N gives a decomposition into paths where each vertex is used once.
    // The number of paths is equal to the number of connected components in the resulting subgraph.
    // Since each path of length L (L vertices) uses L-1 edges, and total vertices = N, total edges = N - K, where K is number of paths.
    // So K = N - (number of edges used) = N - flow2.
    // But flow2 = N (since we pushed N units), so K = 0? That doesn't make sense.

    // The issue: the flow value in this network is the number of edges, not the number of paths.
    // Each path of L vertices uses L-1 edges, and the flow is the sum of edge flows.
    // So if we have K paths, total edges = N - K, so flow = N - K, thus K = N - flow.

    // But we pushed N units from source to sink. How?
    // Each path contributes 1 unit of flow from source to the first out-node, then through edges to in-nodes, etc., to sink.
    // Actually, no: the flow path is source -> out_i -> ... -> in_j -> sink.
    // For a path of L vertices: source -> out_v1 -> in_v2 -> out_v2 -> in_v3 -> ... -> out_vL -> in_vL -> sink.
    // So the flow for one path is 1 unit, but it uses L edges in the original graph (v1->v2, v2->v3, ..., v_{L-1}->vL) and 2L-1 edges in the flow network.
    // The total flow value is the number of paths, not the number of edges.

    // Let me recount:
    // In our network, the capacity constraints on the internal edge (in->out) are 1, so each vertex can be used once.
    // The source connects to each out-node with cap 1, so at most one path can start at a vertex.
    // The in-node connects to sink with cap 1, so at most one path can end at a vertex.
    // Therefore, the flow value is exactly the number of paths.

    // But we forced the flow to be N? That's the mistake. We should not force flow to be N; instead, we want to cover all vertices, 
    // which means each vertex must be incident to exactly one path. In a path, internal vertices have in-degree 1 and out-degree 1, 
    // but endpoints have in-degree 0 or out-degree 0.

    // Actually, the standard model is:
    // We want a set of vertex-disjoint paths that cover all vertices.
    // This is equivalent to a flow where:
    // - Each vertex has in-degree 1 in the path forest (except sources of paths) and out-degree 1 (except sinks).
    // - We minimize the number of paths = number of sources.

    // The minimal number of paths is N - (max matching in the DAG).
    // And to minimize cost, we use min cost flow where we send flow along edges with cost = edge cost.

    // Correct model:
    // Build bipartite graph: left part = vertices, right part = vertices.
    // For each edge u->v in DAG, add edge from u (left) to v (right) with capacity 1, cost = c.
    // Add source connected to all left vertices (cap=1, cost=0), all right vertices connected to sink (cap=1, cost=0).
    // Then a matching of size k corresponds to k edges, and the number of paths = N - k.
    // We want to maximize k (minimize paths) and minimize cost.

    // So we need a maximum matching with minimum cost.

    MinCostFlow mcf3(2 * N + 2);
    int src = 0, dst = 2 * N + 1;

    // Left part: 0..N-1, Right part: N..2N-1
    for (int i = 0; i < M; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        // left a -> right b
        mcf3.addEdge(a, b + N, 1, c);
    }

    // Source to left part
    for (int i = 0; i < N; ++i) {
        mcf3.addEdge(src, i, 1, 0);
    }

    // Right part to sink
    for (int i = 0; i < N; ++i) {
        mcf3.addEdge(i + N, dst, 1, 0);
    }

    // We want maximum matching (so minimal paths = N - |matching|) with minimum cost.
    // So we push as much flow as possible (up to N) and minimize cost.
    auto [flow3, totalCost3] = mcf3.minCostFlow(src, dst, N);

    int numPaths = N - flow3;

    // Now, reconstruct the paths from the flow.
    // For each edge in the matching (i.e., flow on left i to right j), it means j is the next after i in a path.

    vector<int> nextNode(N, -1);
    vector<int> prevNode(N, -1);
    vector<vector<Edge>>& g = mcf3.g;

    for (int u = 0; u < N; ++u) {
        for (auto& e : g[u]) {
            if (e.to >= N && e.to < 2 * N && e.cap == 0 && e.rev < (int)g[e.to].size()) {
                // The edge was used: original capacity was 1, now cap=0 means flow=1
                // But note: in the residual graph, the forward edge cap is 0, and the backward edge has cap=1.
                // We can check the backward edge: g[e.to][e.rev].cap > 0 means flow was sent.
                if (g[e.to][e.rev].cap > 0) {
                    int v = e.to - N;
                    nextNode[u] = v;
                    prevNode[v] = u;
                }
            }
        }
    }

    // Find starts of paths: vertices with no incoming edge in the matching
    vector<int> starts;
    vector<bool> isStart(N, true);
    for (int i = 0; i < N; ++i) {
        if (prevNode[i] != -1) {
            isStart[prevNode[i]] = false;
        }
    }
    for (int i = 0; i < N; ++i) {
        if (isStart[i]) {
            starts.push_back(i);
        }
    }

    // Reconstruct paths
    vector<vector<int>> paths;
    for (int start : starts) {
        vector<int> path;
        int cur = start;
        while (cur != -1) {
            path.push_back(cur);
            cur = nextNode[cur];
        }
        paths.push_back(path);
    }

    // Output
    cout << numPaths << " " << totalCost3 << "\n";
    for (auto& path : paths) {
        cout << path.size();
        for (int node : path) {
            cout << " " << node + 1;
        }
        cout << "\n";
    }

    return 0;
}
