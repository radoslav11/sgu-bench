#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<string> grid(n);
    vector<pair<int,int>> rocks;
    
    for(int i = 0; i < n; i++){
        cin >> grid[i];
        for(int j = 0; j < m; j++){
            if(grid[i][j] == '*'){
                rocks.push_back({i, j});
            }
        }
    }
    
    int numRocks = rocks.size();
    
    if(numRocks == 0){
        cout << 0 << endl;
        return 0;
    }
    
    // For each cell (i,j), precompute which rocks it destroys (as a bitmask)
    vector<vector<long long>> destroys(n, vector<long long>(m, 0));
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            long long mask = 0;
            for(int k = 0; k < numRocks; k++){
                if(rocks[k].first == i || rocks[k].second == j){
                    mask |= (1LL << k);
                }
            }
            destroys[i][j] = mask;
        }
    }
    
    // BFS to find minimum bombs
    map<long long, int> dist;
    queue<long long> q;
    
    long long allRocks = (1LL << numRocks) - 1;
    q.push(0);
    dist[0] = 0;
    
    while(!q.empty()){
        long long state = q.front();
        q.pop();
        
        if(state == allRocks){
            cout << dist[state] << endl;
            return 0;
        }
        
        int currDist = dist[state];
        
        // Try dropping a bomb at each cell
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                long long newState = state | destroys[i][j];
                if(dist.find(newState) == dist.end()){
                    dist[newState] = currDist + 1;
                    q.push(newState);
                }
            }
        }
    }
    
    cout << 0 << endl;
    return 0;
}
