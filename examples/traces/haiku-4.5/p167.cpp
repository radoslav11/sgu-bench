#include<bits/stdc++.h>
using namespace std;

int N, M, K;
int grid[15][15];
int maxOil = -1;
vector<pair<int,int>> bestSolution;

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < M;
}

bool isConnected(set<pair<int,int>>& cells) {
    if(cells.empty()) return true;
    
    set<pair<int,int>> visited;
    queue<pair<int,int>> q;
    q.push(*cells.begin());
    visited.insert(*cells.begin());
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while(!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        
        for(int i = 0; i < 4; i++) {
            int nr = r + dx[i];
            int nc = c + dy[i];
            if(isValid(nr, nc) && cells.count({nr, nc}) && !visited.count({nr, nc})) {
                visited.insert({nr, nc});
                q.push({nr, nc});
            }
        }
    }
    
    return visited.size() == cells.size();
}

void solve(int sr, int sc) {
    set<pair<int,int>> selected;
    selected.insert({sr, sc});
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while(selected.size() < K) {
        pair<int,int> best = {-1, -1};
        int bestOil = -1;
        
        for(auto [r, c] : selected) {
            for(int i = 0; i < 4; i++) {
                int nr = r + dx[i];
                int nc = c + dy[i];
                if(isValid(nr, nc) && !selected.count({nr, nc})) {
                    if(grid[nr][nc] > bestOil) {
                        bestOil = grid[nr][nc];
                        best = {nr, nc};
                    }
                }
            }
        }
        
        if(best.first == -1) break;
        selected.insert(best);
    }
    
    if(selected.size() == K) {
        int oil = 0;
        for(auto [r, c] : selected) {
            oil += grid[r][c];
        }
        
        if(oil > maxOil) {
            maxOil = oil;
            bestSolution.clear();
            for(auto p : selected) {
                bestSolution.push_back(p);
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> K;
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            cin >> grid[i][j];
        }
    }
    
    if(K == 0) {
        cout << "Oil : 0\n";
        return 0;
    }
    
    // Try starting from each cell
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            solve(i, j);
        }
    }
    
    cout << "Oil : " << maxOil << "\n";
    for(auto [r, c] : bestSolution) {
        cout << (r+1) << " " << (c+1) << "\n";
    }
    
    return 0;
}
