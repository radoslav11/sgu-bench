#include<bits/stdc++.h>
using namespace std;

int N, K;
vector<int> adj[30005];
vector<int> result;

int dfs(int u, int parent) {
    int maxDist = 0;
    
    for(int v : adj[u]) {
        if(v == parent) continue;
        int childDist = dfs(v, u);
        if(childDist >= 0) {
            maxDist = max(maxDist, childDist + 1);
        } else {
            maxDist = max(maxDist, 1);
        }
    }
    
    if(maxDist > K) {
        result.push_back(u);
        return -1;
    }
    return maxDist;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    
    for(int i = 0; i < N-1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    
    int rootDist = dfs(1, -1);
    if(rootDist >= 0) result.push_back(1);
    
    cout << result.size() << "\n";
    for(int x : result) cout << x << "\n";
    
    return 0;
}
