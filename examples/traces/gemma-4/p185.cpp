#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>

using namespace std;

/**
 * Problem: Find two edge-disjoint shortest paths from junction 1 to junction N.
 * 
 * Approach:
 * 1. Use Dijkstra's algorithm to find the shortest distance from node 1 to all other nodes (dist1)
 *    and from node N to all other nodes (distN).
 * 2. A road (u, v) of length L is part of some shortest path from 1 to N if:
 *    dist1[u] + L + distN[v] == dist1[N] OR dist1[v] + L + distN[u] == dist1[N].
 * 3. Build a directed graph G_shortest consisting of these "useful" edges.
 * 4. Each edge in G_shortest has capacity 1.
 * 5. Find if the max flow from 1 to N in G_shortest is at least 2.
 * 6. If yes, reconstruct the two edge-disjoint paths using the flow.
 * 
 * Constraints: N <= 400, L <= 10000. Shortest path can be up to 4,000,000. 
 * Using long long for distances and capacities ensures no overflow.
 * Time limit: 0.25s is tight, so use fast I/O and an efficient implementation.
 */

const long long INF = 1e16;

struct Edge {
    int to;
    long long cap, flow;
    int rev;
};

vector<Edge> adj[405];
long long dist1[405], distN[405];

void add_edge(int u, int v, long long cap) {
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1});
}

void dijkstra(int start, int n, long long dist[], const vector<pair<int, int>> original_adj[]) {
    for (int i = 1; i <= n; ++i) dist[i] = INF;
    dist[start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : original_adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

int parent_node[405];
int parent_edge_idx[405];

bool bfs(int s, int t, int n) {
    for (int i = 1; i <= n; ++i) parent_node[i] = -1;
    queue<int> q;
    q.push(s);
    parent_node[s] = s;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < (int)adj[u].size(); ++i) {
            if (parent_node[adj[u][i].to] == -1 && adj[u][i].cap - adj[u][i].flow > 0) {
                parent_node[adj[u][i].to] = u;
                parent_edge_idx[adj[u][i].to] = i;
                q.push(adj[u][i].to);
                if (adj[u][i].to == t) return true;
            }
        }
    }
    return false;
}

struct InputEdge {
    int u, v, l;
};

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<pair<int, int>> original_adj[405];
    vector<InputEdge> input_edges;

    for (int i = 0; i < m; ++i) {
        int u, v, l;
        if (scanf("%d %d %d", &u, &v, &l) != 3) break;
        original_adj[u].push_back({v, l});
        original_adj[v].push_back({u, l});
        input_edges.push_back({u, v, l});
    }

    dijkstra(1, n, dist1, original_adj);
    dijkstra(n, n, distN, original_adj);

    if (dist1[n] == INF) {
        printf("No solution\n");
        return 0;
    }

    long long shortest_dist = dist1[n];

    // Build G_shortest where every edge is part of a shortest path from 1 to N
    for (const auto& e : input_edges) {
        if (dist1[e.u] + (long long)e.l + distN[e.v] == shortest_dist) {
            add_edge(e.u, e.v, 1);
        } else if (dist1[e.v] + (long long)e.l + distN[e.u] == shortest_dist) {
            add_edge(e.v, e.u, 1);
        }
    }

    // Edmonds-Karp to find up to 2 augmenting paths
    int max_flow = 0;
    while (max_flow < 2 && bfs(1, n, n)) {
        max_flow++;
        int curr = n;
        while (curr != 1) {
            int prev = parent_node[curr];
            int idx = parent_edge_idx[curr];
            adj[prev][idx].flow += 1;
            int rev_idx = adj[prev][idx].rev;
            adj[curr][rev_idx].flow -= 1;
            curr = prev;
        }
    }

    if (max_flow < 2) {
        printf("No solution\n");
    } else {
        vector<int> path1, path2;
        
        auto get_path = [&](int s, int t, vector<int>& path) {
            int curr = s;
            path.push_back(curr);
            while (curr != t) {
                bool found = false;
                for (auto& e : adj[curr]) {
                    // If it's a forward edge with flow 1, follow it
                    if (e.cap == 1 && e.flow == 1) {
                        e.flow = 0; // Mark it used so we don't take it for the next path
                        curr = e.to;
                        path.push_back(curr);
                        found = true;
                        break;
                    }
                }
                if (!found) break;
            }
        };

        get_path(1, n, path1);
        get_path(1, n, path2);

        for (int i = 0; i < (int)path1.size(); ++i) {
            printf("%d%c", path1[i], (i == (int)path1.size() - 1 ? '\n' : ' '));
        }
        for (int i = 0; i < (int)path2.size(); ++i) {
            printf("%d%c", path2[i], (i == (int)path2.size() - 1 ? '\n' : ' '));
        }
    }

    return 0;
}

