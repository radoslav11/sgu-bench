#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N + 1);
    vector<int> indegree(N + 1, 0);
    
    for (int i = 0; i < M; i++) {
        int p, q;
        cin >> p >> q;
        // box p lighter than box q: value[p] < value[q]
        // edge p -> q
        adj[p].push_back(q);
        indegree[q]++;
    }
    
    // Topological sort (Kahn's algorithm)
    queue<int> q;
    for (int i = 1; i <= N; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> topo;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        topo.push_back(node);
        for (int next : adj[node]) {
            indegree[next]--;
            if (indegree[next] == 0) {
                q.push(next);
            }
        }
    }
    
    if ((int)topo.size() != N) {
        cout << "No solution" << endl;
        return 0;
    }
    
    // topo[0] gets value 1, topo[1] gets value 2, etc.
    vector<int> ans(N + 1);
    for (int i = 0; i < N; i++) {
        ans[topo[i]] = i + 1;
    }
    
    for (int i = 1; i <= N; i++) {
        cout << ans[i];
        if (i < N) cout << " ";
    }
    cout << endl;
    
    return 0;
}
