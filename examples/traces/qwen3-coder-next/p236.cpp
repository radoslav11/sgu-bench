#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

struct Edge {
    int to;
    int cost;
    int time;
};

const double INF = 1e18;
const double NEG_INF = -1e18;
const double EPS = 1e-9;

bool check(double R, int n, const vector<vector<Edge>>& edges, vector<int>& cycle) {
    vector<vector<double>> d(51, vector<double>(n, NEG_INF));
    vector<vector<int>> parent(51, vector<int>(n, -1));

    for (int v = 0; v < n; v++) {
        d[0][v] = 0;
    }

    for (int k = 1; k <= n; k++) {
        for (int u = 0; u < n; u++) {
            if (d[k-1][u] == NEG_INF) continue;
            for (const Edge& e : edges[u]) {
                double weight = e.cost - R * e.time;
                if (d[k-1][u] + weight > d[k][e.to]) {
                    d[k][e.to] = d[k-1][u] + weight;
                    parent[k][e.to] = u;
                }
            }
        }
    }

    for (int v = 0; v < n; v++) {
        for (int k = 0; k < n; k++) {
            if (d[n][v] - d[k][v] >= -EPS) {
                vector<int> path;
                int cur = v;
                for (int i = n; i >= 1; i--) {
                    path.push_back(cur + 1);
                    cur = parent[i][cur];
                }
                path.push_back(cur + 1);
                reverse(path.begin(), path.end());

                int first_occurrence = -1;
                for (int i = 0; i < n; i++) {
                    if (path[i] == v + 1) {
                        first_occurrence = i;
                        break;
                    }
                }
                if (first_occurrence == -1) {
                    continue;
                }
                vector<int> cycle_vertices;
                for (int i = first_occurrence; i < n; i++) {
                    cycle_vertices.push_back(path[i]);
                }
                cycle = cycle_vertices;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<Edge>> edges(n);
    for (int i = 0; i < m; i++) {
        int a, b, c, t;
        cin >> a >> b >> c >> t;
        a--; b--;
        edges[a].push_back({b, c, t});
    }

    double low = 0.0;
    double high = 5000.0;
    vector<int> best_cycle;

    for (int iter = 0; iter < 50; iter++) {
        double mid = (low + high) / 2.0;
        vector<int> cycle;
        if (check(mid, n, edges, cycle)) {
            best_cycle = cycle;
            low = mid;
        } else {
            high = mid;
        }
    }

    if (best_cycle.empty()) {
        cout << "0" << endl;
    } else {
        cout << best_cycle.size() << endl;
        for (int i = 0; i < best_cycle.size(); i++) {
            if (i > 0) cout << " ";
            cout << best_cycle[i];
        }
        cout << endl;
    }

    return 0;
}
