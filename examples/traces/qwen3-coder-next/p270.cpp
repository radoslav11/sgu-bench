#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<bool> visited(n + 1, false);
    vector<int> component;
    
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            vector<int> comp;
            // BFS for component starting at i
            if (i == 1) {
                vector<bool> vis(n + 1, false);
                vector<int> q;
                q.push_back(1);
                vis[1] = true;
                while (!q.empty()) {
                    int u = q.back();
                    q.pop_back();
                    comp.push_back(u);
                    for (int v : adj[u]) {
                        if (!vis[v]) {
                            vis[v] = true;
                            q.push_back(v);
                        }
                    }
                }
                component = comp;
                break;
            }
        }
    }

    vector<int> ans;
    for (int i = 1; i <= n; i++) {
        if (find(component.begin(), component.end(), i) != component.end()) {
            ans.push_back(i);
        }
    }

    for (int i = 0; i < ans.size(); i++) {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << endl;

    return 0;
}
