#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    vector<pair<long long, long long>> points;
    set<long long> xs, ys;
    
    for (int i = 0; i < K; i++) {
        long long x, y;
        cin >> x >> y;
        points.push_back({x, y});
        xs.insert(x);
        ys.insert(y);
    }
    
    // Map coordinates to indices
    map<long long, int> x_to_idx;
    map<long long, int> y_to_idx;
    
    int idx = 0;
    for (long long x : xs) {
        x_to_idx[x] = idx++;
    }
    
    int num_x = idx;
    for (long long y : ys) {
        y_to_idx[y] = idx++;
    }
    
    int total_nodes = num_x + (int)ys.size();
    vector<vector<int>> adj(total_nodes);
    
    // Build bipartite graph
    for (auto& p : points) {
        int x_idx = x_to_idx[p.first];
        int y_idx = y_to_idx[p.second];
        // x node connects to y node
        adj[x_idx].push_back(y_idx);
        adj[y_idx].push_back(x_idx);
    }
    
    vector<bool> visited(total_nodes, false);
    long long result = 0;
    
    for (int i = 0; i < total_nodes; i++) {
        if (!visited[i]) {
            queue<int> q;
            q.push(i);
            visited[i] = true;
            
            int cnt_x = 0, cnt_y = 0;
            
            if (i < num_x) {
                cnt_x++;
            } else {
                cnt_y++;
            }
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                        
                        if (v < num_x) {
                            cnt_x++;
                        } else {
                            cnt_y++;
                        }
                    }
                }
            }
            
            result += (long long)cnt_x * cnt_y;
        }
    }
    
    cout << result << endl;
    
    return 0;
}
