#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N + 1);
    
    for (int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        if (a != b) {
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
    }
    
    vector<int> color(N + 1, -1);
    bool possible = true;
    
    for (int i = 1; i <= N && possible; i++) {
        if (color[i] == -1) {
            // BFS
            queue<int> q;
            q.push(i);
            color[i] = 0;
            while (!q.empty() && possible) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        possible = false;
                    }
                }
            }
        }
    }
    
    if (!possible) {
        cout << "no" << endl;
    } else {
        cout << "yes" << endl;
        vector<int> day1;
        for (int i = 1; i <= N; i++) {
            if (color[i] == 0) {
                day1.push_back(i);
            }
        }
        cout << day1.size() << endl;
        for (int i = 0; i < (int)day1.size(); i++) {
            if (i > 0) cout << " ";
            cout << day1[i];
        }
        cout << endl;
    }
    
    return 0;
}
