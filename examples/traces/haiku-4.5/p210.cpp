#include<bits/stdc++.h>
using namespace std;

int n;
vector<long long> love;
vector<vector<int>> prefs;
vector<int> assignment;
vector<int> sonAssignment;

bool dfs(int son, vector<bool>& visited) {
    for(int girl : prefs[son]) {
        girl--;
        if(visited[girl]) continue;
        visited[girl] = true;
        
        if(assignment[girl] == -1 || dfs(assignment[girl], visited)) {
            assignment[girl] = son;
            sonAssignment[son] = girl + 1;
            return true;
        }
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    love.resize(n);
    prefs.resize(n);
    assignment.assign(n, -1);
    sonAssignment.assign(n, 0);
    
    for(int i = 0; i < n; i++) {
        cin >> love[i];
    }
    
    for(int i = 0; i < n; i++) {
        int k;
        cin >> k;
        prefs[i].resize(k);
        for(int j = 0; j < k; j++) {
            cin >> prefs[i][j];
        }
    }
    
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [](int a, int b) {
        return love[a] > love[b];
    });
    
    for(int son : order) {
        vector<bool> visited(n, false);
        dfs(son, visited);
    }
    
    for(int i = 0; i < n; i++) {
        if(i > 0) cout << " ";
        cout << sonAssignment[i];
    }
    cout << "\n";
    
    return 0;
}
