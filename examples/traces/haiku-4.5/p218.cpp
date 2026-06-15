#include<bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> unsafety;
vector<int> match;
vector<bool> used;

bool dfs(int v, int maxVal, vector<vector<int>>& adj) {
    for(int to : adj[v]) {
        if(used[to]) continue;
        used[to] = true;
        if(match[to] == -1 || dfs(match[to], maxVal, adj)) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

bool canAssign(int maxVal) {
    // Build bipartite graph where edge exists if unsafety <= maxVal
    vector<vector<int>> adj(n);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(unsafety[i][j] <= maxVal) {
                adj[i].push_back(j);
            }
        }
    }
    
    match.assign(n, -1);
    int matched = 0;
    
    for(int i = 0; i < n; i++) {
        used.assign(n, false);
        if(dfs(i, maxVal, adj)) {
            matched++;
        }
    }
    
    return matched == n;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    unsafety.resize(n, vector<int>(n));
    
    vector<int> allValues;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> unsafety[i][j];
            allValues.push_back(unsafety[i][j]);
        }
    }
    
    sort(allValues.begin(), allValues.end());
    allValues.erase(unique(allValues.begin(), allValues.end()), allValues.end());
    
    int answer = allValues.back();
    
    // Binary search on answer
    int left = 0, right = allValues.size() - 1;
    while(left <= right) {
        int mid = (left + right) / 2;
        if(canAssign(allValues[mid])) {
            answer = allValues[mid];
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    // Find the actual assignment with this answer
    vector<vector<int>> adj(n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(unsafety[i][j] <= answer) {
                adj[i].push_back(j);
            }
        }
    }
    
    match.assign(n, -1);
    for(int i = 0; i < n; i++) {
        used.assign(n, false);
        dfs(i, answer, adj);
    }
    
    // match[j] = i means computer i runs program j
    vector<int> assignment(n);
    for(int j = 0; j < n; j++) {
        assignment[match[j]] = j;
    }
    
    cout << answer << "\n";
    for(int i = 0; i < n; i++) {
        cout << (i + 1) << " " << (assignment[i] + 1) << "\n";
    }
    
    return 0;
}
