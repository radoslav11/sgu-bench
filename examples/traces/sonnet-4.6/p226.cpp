#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    // Input might have "Test #X" lines, need to handle that
    string line;
    // Read until we get N and M
    // Actually let's just try to parse carefully
    
    // Read all tokens
    // The input has "Test #1" etc. Let's just read tokens and skip non-numeric or handle
    // We'll read line by line and try to parse
    
    // Actually let's just read tokens, skipping non-integer tokens
    vector<int> nums;
    string token;
    while(cin >> token){
        // try to parse as int
        bool isnum = true;
        int start = 0;
        if(token[0] == '-') start = 1;
        if(start == token.size()) isnum = false;
        for(int i = start; i < (int)token.size(); i++){
            if(!isdigit(token[i])){ isnum = false; break; }
        }
        if(isnum){
            nums.push_back(stoi(token));
        }
    }
    
    int idx = 0;
    N = nums[idx++];
    M = nums[idx++];
    
    // adj[v] = list of (u, color)
    vector<vector<pair<int,int>>> adj(N+1);
    
    for(int i = 0; i < M; i++){
        int x = nums[idx++];
        int y = nums[idx++];
        int c = nums[idx++];
        adj[x].push_back({y, c});
    }
    
    // BFS: state = (vertex, last_color) where last_color in {0,1,2,3}, 0 = no previous
    // dist[v][c] = shortest path length to reach v with last edge color c
    vector<vector<int>> dist(N+1, vector<int>(4, INT_MAX));
    
    queue<pair<int,int>> q;
    dist[1][0] = 0;
    q.push({1, 0});
    
    while(!q.empty()){
        auto [v, lc] = q.front(); q.pop();
        int d = dist[v][lc];
        
        for(auto [u, c] : adj[v]){
            if(c == lc) continue; // same color not allowed
            if(dist[u][c] == INT_MAX){
                dist[u][c] = d + 1;
                q.push({u, c});
            }
        }
    }
    
    int ans = INT_MAX;
    for(int c = 0; c <= 3; c++){
        ans = min(ans, dist[N][c]);
    }
    
    if(ans == INT_MAX) cout << -1 << endl;
    else cout << ans << endl;
    
    return 0;
}
