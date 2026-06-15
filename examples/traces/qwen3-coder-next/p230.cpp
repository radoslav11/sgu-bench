#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N + 1);
    vector<int> in_degree(N + 1, 0);

    for (int i = 0; i < M; i++) {
        int P, Q;
        cin >> P >> Q;
        adj[P].push_back(Q);
        in_degree[Q]++;
    }

    queue<int> q;
    for (int i = 1; i <= N; i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    vector<int> topo_order;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);

        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (topo_order.size() != N) {
        cout << "No solution" << endl;
    } else {
        vector<int> ans(N + 1);
        for (int i = 0; i < N; i++) {
            ans[topo_order[i]] = i + 1;
        }

        for (int i = 1; i <= N; i++) {
            if (i > 1) cout << " ";
            cout << ans[i];
        }
        cout << endl;
    }

    return 0;
}
