#include<bits/stdc++.h>
using namespace std;

int N, M;
vector<tuple<int, int, int>> adj[55]; // {destination, cost, time}
vector<int> bestPath;
double bestRatio = -1;

void dfs(int start, int current, vector<int>& path, vector<bool>& visited, int totalCost, int totalTime) {
    // Try all outgoing edges from current
    for(auto& [next, cost, time] : adj[current]) {
        if(next == start && path.size() >= 2) {
            // Found a cycle back to start
            double ratio = (double)(totalCost + cost) / (totalTime + time);
            if(ratio > bestRatio) {
                bestRatio = ratio;
                bestPath = path;
                bestPath.push_back(start);
            }
        }
        else if(!visited[next] && path.size() < 50) {
            // Continue exploring
            visited[next] = true;
            path.push_back(next);
            dfs(start, next, path, visited, totalCost + cost, totalTime + time);
            path.pop_back();
            visited[next] = false;
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    
    for(int i = 0; i < M; i++) {
        int a, b, cost, time;
        cin >> a >> b >> cost >> time;
        adj[a].push_back({b, cost, time});
    }
    
    // Try starting from each node
    for(int start = 1; start <= N; start++) {
        vector<bool> visited(N + 1, false);
        vector<int> path;
        visited[start] = true;
        path.push_back(start);
        dfs(start, start, path, visited, 0, 0);
    }
    
    if(bestPath.empty()) {
        cout << "0\n";
    } else {
        cout << bestPath.size() << "\n";
        for(int i = 0; i < bestPath.size(); i++) {
            if(i > 0) cout << " ";
            cout << bestPath[i];
        }
        cout << "\n";
    }
    
    return 0;
}
