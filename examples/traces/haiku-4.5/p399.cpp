#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> grid(N);
    for(int i = 0; i < N; i++){
        cin >> grid[i];
    }
    
    // Distance from ocean for each cell
    vector<vector<int>> dist(N, vector<int>(M, INT_MAX));
    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(M, {-1,-1}));
    
    queue<pair<int,int>> q;
    
    // Start BFS from all border cells that are '.'
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(grid[i][j] == '.' && (i == 0 || i == N-1 || j == 0 || j == M-1)){
                q.push({i, j});
                dist[i][j] = 0;
            }
        }
    }
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    // BFS to find distances to ocean
    while(!q.empty()){
        auto [x, y] = q.front();
        q.pop();
        
        for(int d = 0; d < 4; d++){
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if(nx >= 0 && nx < N && ny >= 0 && ny < M && grid[nx][ny] == '.' && dist[nx][ny] == INT_MAX){
                dist[nx][ny] = dist[x][y] + 1;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }
    
    // Find all craters and their min distance to ocean
    vector<vector<int>> crater_id(N, vector<int>(M, -1));
    vector<int> crater_min_dist;
    int num_craters = 0;
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(grid[i][j] == 'X' && crater_id[i][j] == -1){
                // BFS to find crater
                queue<pair<int,int>> bfs;
                bfs.push({i, j});
                crater_id[i][j] = num_craters;
                int min_d = INT_MAX;
                
                while(!bfs.empty()){
                    auto [x, y] = bfs.front();
                    bfs.pop();
                    
                    // Check adjacent empty cells
                    for(int d = 0; d < 4; d++){
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if(nx >= 0 && nx < N && ny >= 0 && ny < M){
                            if(grid[nx][ny] == 'X' && crater_id[nx][ny] == -1){
                                crater_id[nx][ny] = num_craters;
                                bfs.push({nx, ny});
                            }
                            else if(grid[nx][ny] == '.' && dist[nx][ny] != INT_MAX){
                                min_d = min(min_d, dist[nx][ny] + 1);
                            }
                        }
                    }
                }
                
                crater_min_dist.push_back(min_d);
                num_craters++;
            }
        }
    }
    
    // Find 2 craters with minimum distance
    int c1 = -1, c2 = -1;
    int min_sum = INT_MAX;
    for(int i = 0; i < num_craters; i++){
        for(int j = i+1; j < num_craters; j++){
            if(crater_min_dist[i] + crater_min_dist[j] < min_sum){
                min_sum = crater_min_dist[i] + crater_min_dist[j];
                c1 = i;
                c2 = j;
            }
        }
    }
    
    // Trace paths from both craters to ocean and mark
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(grid[i][j] == '.' && dist[i][j] != INT_MAX){
                if(dist[i][j] > 0){
                    // Check if this cell is on path to c1 or c2
                    bool on_path = false;
                    for(int d = 0; d < 4; d++){
                        int ni = i + dx[d];
                        int nj = j + dy[d];
                        if(ni >= 0 && ni < N && nj >= 0 && nj < M){
                            if((crater_id[ni][nj] == c1 || crater_id[ni][nj] == c2) && dist[i][j] == crater_min_dist[crater_id[ni][nj]] - 1){
                                on_path = true;
                                break;
                            }
                        }
                    }
                    if(on_path) grid[i][j] = '*';
                }
            }
        }
    }
    
    for(int i = 0; i < N; i++){
        cout << grid[i] << "\n";
    }
    
    return 0;
}
