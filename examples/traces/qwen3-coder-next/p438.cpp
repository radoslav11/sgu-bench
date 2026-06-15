#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

struct Pile {
    int x, y, c;
};

bool feasible(int T, int M, int D, int W, const vector<Pile>& piles) {
    int N = piles.size();
    const long long INF = 1000000;
    vector<long long> prev_cur(N, 0);
    long long total_flow = 0;

    for (int t = 1; t <= T; t++) {
        int nodes = 2 * N + 2;
        vector<vector<long long>> cap(nodes, vector<long long>(nodes, 0));
        int sink_node = nodes - 1;

        for (int i = 0; i < N; i++) {
            cap[0][i + 1] = M + prev_cur[i];
            cap[i + 1][N + 1 + i] = piles[i].c;
            if (W - piles[i].y <= D) {
                cap[N + 1 + i][sink_node] = INF;
            }
            for (int j = 0; j < N; j++) {
                long long dx = piles[i].x - piles[j].x;
                long long dy = piles[i].y - piles[j].y;
                if (dx * dx + dy * dy <= (long long)D * D) {
                    cap[N + 1 + i][j + 1] = INF;
                }
            }
        }

        if (W <= D) {
            cap[0][sink_node] = INF;
        }

        long long flow_value = 0;
        while (true) {
            vector<int> parent(nodes, -1);
            queue<int> q;
            q.push(0);
            parent[0] = -2;
            while (!q.empty() && parent[sink_node] == -1) {
                int u = q.front();
                q.pop();
                for (int v = 0; v < nodes; v++) {
                    if (parent[v] == -1 && cap[u][v] > 0) {
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }
            if (parent[sink_node] == -1) break;
            long long path_flow = INF;
            int v = sink_node;
            while (v != 0) {
                int u = parent[v];
                path_flow = min(path_flow, cap[u][v]);
                v = u;
            }
            v = sink_node;
            while (v != 0) {
                int u = parent[v];
                cap[u][v] -= path_flow;
                cap[v][u] += path_flow;
                v = u;
            }
            flow_value += path_flow;
        }

        total_flow += flow_value;
        if (total_flow >= M) {
            return true;
        }

        vector<long long> next_cur(N, 0);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (cap[N + 1 + i][j + 1] < INF) {
                    long long flow_edge = INF - cap[N + 1 + i][j + 1];
                    next_cur[j] += flow_edge;
                }
            }
        }
        prev_cur = next_cur;
    }

    return total_flow >= M;
}

int main() {
    int N, M, D, W;
    cin >> N >> M >> D >> W;

    vector<Pile> piles(N);
    for (int i = 0; i < N; i++) {
        cin >> piles[i].x >> piles[i].y >> piles[i].c;
    }

    if (M == 0) {
        cout << 0 << endl;
        return 0;
    }

    int low = 1;
    int high = 2000;
    int ans = -1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (feasible(mid, M, D, W, piles)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    if (ans == -1) {
        cout << "IMPOSSIBLE" << endl;
    } else {
        cout << ans << endl;
    }

    return 0;
}
