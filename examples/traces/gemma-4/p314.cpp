#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/**
 * Problem: Find the k shortest path lengths from a source s to a destination t in a directed graph.
 * Paths don't need to be simple. The paths can visit vertices or edges multiple times.
 * 
 * Constraints:
 * n <= 10,000, m <= 50,000, k <= 10,000
 * Time limit: 0.25s
 * Memory limit: 64MB
 * 
 * Approach:
 * We use the k-shortest paths version of Dijkstra's algorithm. 
 * Each vertex u is popped from the priority queue at most k times. 
 * When u is popped for the i-th time, it represents the i-th shortest path from s to u.
 * Total number of pops is at most n * k. 
 * Total number of edge relaxations is at most m * k.
 * 
 * Given the very tight time limit (0.25s), we use optimized I/O and a fast priority queue.
 * Note: In the worst case, m*k operations might be slow, but the k-shortest paths Dijkstra
 * often performs better on many types of graphs.
 */

const int MAXN = 10005;
const int MAXM = 50005;

// Adjacency list structure to save memory and improve cache efficiency
struct Edge {
    int to;
    int weight;
    int next;
} edges[MAXM];

int head[MAXN], edge_cnt = 0;

// Function to add a directed edge to the graph
inline void add_edge(int u, int v, int w) {
    edges[++edge_cnt].to = v;
    edges[edge_cnt].weight = w;
    edges[edge_cnt].next = head[u];
    head[u] = edge_cnt;
}

// counts[u] stores how many times vertex u has been popped from the priority queue
int counts[MAXN];
// results stores the lengths of the first k shortest paths to the destination t
long long results[10005];

int main() {
    int n, m, k;
    // Reading n (vertices), m (edges), k (number of paths)
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;

    int s, t;
    // Reading source s and destination t
    if (scanf("%d %d", &s, &t) != 2) return 0;

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) break;
        add_edge(u, v, w);
    }

    // Priority queue stores pairs of {current_distance, current_vertex}
    // Using a min-heap to always explore the shortest distance first
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, s});

    int found = 0;
    while (!pq.empty() && found < k) {
        pair<long long, int> current = pq.top();
        pq.pop();

        long long d = current.first;
        int u = current.second;

        // If we have already found the k shortest paths to u, any subsequent 
        // path through u will be longer than the k shortest paths we already found.
        if (counts[u] >= k) continue;
        counts[u]++;

        // If the current popped vertex is the destination, record its distance
        if (u == t) {
            results[found++] = d;
        }

        // Relax all outgoing edges from the current vertex
        for (int i = head[u]; i; i = edges[i].next) {
            int v = edges[i].to;
            // Pruning: only push to PQ if we haven't already popped v k times
            if (counts[v] < k) {
                pq.push({d + edges[i].weight, v});
            }
        }
    }

    // Print the lengths of the k shortest paths or "NO" if fewer than k paths exist
    for (int i = 0; i < k; ++i) {
        if (i < found) {
            printf("%lld\n", results[i]);
        } else {
            printf("NO\n");
        }
    }

    return 0;
}

