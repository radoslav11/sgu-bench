#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<vector<int>> graph(N + 1);
    vector<bool> visited(N + 1, false);
    vector<int> path;
    
    // Read adjacency list
    for (int i = 1; i <= N; i++) {
        int friend_node;
        while (cin >> friend_node) {
            graph[i].push_back(friend_node);
        }
    }
    
    // DFS to find Hamiltonian cycle
    path.push_back(1);
    visited[1] = true;
    
    function<bool()> dfs = [&]() -> bool {
        if (path.size() == N) {
            // Check if last node connects back to 1
            for (int neighbor : graph[path.back()]) {
                if (neighbor == 1) {
                    path.push_back(1);
                    return true;
                }
            }
            return false;
        }
        
        int current = path.back();
        for (int next : graph[current]) {
            if (!visited[next]) {
                visited[next] = true;
                path.push_back(next);
                
                if (dfs()) {
                    return true;
                }
                
                path.pop_back();
                visited[next] = false;
            }
        }
        
        return false;
    };
    
    if (dfs()) {
        for (int i = 0; i < path.size(); i++) {
            if (i > 0) cout << " ";
            cout << path[i];
        }
        cout << endl;
    } else {
        cout << "No solution" << endl;
    }
    
    return 0;
}
