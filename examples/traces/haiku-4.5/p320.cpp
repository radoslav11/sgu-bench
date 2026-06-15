#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++){
        cin >> grid[i];
    }
    
    // Find connected components and their sizes
    vector<vector<int>> component(N, vector<int>(M, -1));
    vector<int> compSize;
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(component[i][j] == -1){
                int compId = compSize.size();
                compSize.push_back(0);
                queue<pair<int,int>> q;
                q.push({i, j});
                component[i][j] = compId;
                char digit = grid[i][j];
                
                while(!q.empty()){
                    auto [x, y] = q.front();
                    q.pop();
                    compSize[compId]++;
                    
                    for(int d = 0; d < 4; d++){
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if(nx >= 0 && nx < N && ny >= 0 && ny < M && 
                           component[nx][ny] == -1 && grid[nx][ny] == digit){
                            component[nx][ny] = compId;
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
    }
    
    // Identify big leaders
    set<int> bigLeaders;
    for(int i = 0; i < compSize.size(); i++){
        if(compSize[i] >= K){
            bigLeaders.insert(i);
        }
    }
    
    // Mark dangerous squares
    set<pair<int,int>> dangerous;
    
    // All squares of big leaders are dangerous
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(bigLeaders.count(component[i][j])){
                dangerous.insert({i, j});
            }
        }
    }
    
    // Find squares that can't reach border without crossing big leaders
    vector<vector<bool>> canReachBorder(N, vector<bool>(M, false));
    queue<pair<int,int>> q;
    
    // Start BFS from border squares that are NOT in big leader regions
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if((i == 0 || i == N-1 || j == 0 || j == M-1) && 
               !bigLeaders.count(component[i][j])){
                canReachBorder[i][j] = true;
                q.push({i, j});
            }
        }
    }
    
    while(!q.empty()){
        auto [x, y] = q.front();
        q.pop();
        
        for(int d = 0; d < 4; d++){
            int nx = x + dx[d];
            int ny = y + dy[d];
            if(nx >= 0 && nx < N && ny >= 0 && ny < M && 
               !canReachBorder[nx][ny] && !bigLeaders.count(component[nx][ny])){
                canReachBorder[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
    
    // Mark unreachable squares as dangerous
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(!canReachBorder[i][j] && !bigLeaders.count(component[i][j])){
                dangerous.insert({i, j});
            }
        }
    }
    
    cout << dangerous.size() << endl;
    
    return 0;
}
