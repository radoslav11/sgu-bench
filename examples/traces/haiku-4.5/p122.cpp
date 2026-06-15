#include<bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> friends;
vector<int> path;
vector<bool> visited;

bool dfs(int current, int start, int depth) {
    if (depth == N) {
        // Check if current is connected back to start (node 1)
        for (int f : friends[current]) {
            if (f == start) {
                path.push_back(start);
                return true;
            }
        }
        return false;
    }
    
    for (int next : friends[current]) {
        if (!visited[next]) {
            visited[next] = true;
            path.push_back(next);
            
            if (dfs(next, start, depth + 1)) {
                return true;
            }
            
            path.pop_back();
            visited[next] = false;
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    friends.resize(N + 1);
    visited.resize(N + 1, false);
    
    for (int i = 1; i <= N; i++) {
        int f;
        while (cin >> f) {
            friends[i].push_back(f);
            if (cin.peek() == '\n' || cin.peek() == EOF) break;
        }
    }
    
    visited[1] = true;
    path.push_back(1);
    
    if (dfs(1, 1, 1)) {
        for (int i = 0; i < path.size(); i++) {
            if (i > 0) cout << " ";
            cout << path[i];
        }
        cout << "\n";
    } else {
        cout << "No solution\n";
    }
    
    return 0;
}
