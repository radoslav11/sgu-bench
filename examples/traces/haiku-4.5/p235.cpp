#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> board(N);
    int startX = -1, startY = -1;
    
    for(int i = 0; i < N; i++){
        cin >> board[i];
        for(int j = 0; j < N; j++){
            if(board[i][j] == 'Q'){
                startX = i;
                startY = j;
            }
        }
    }
    
    // dp[m][i][j] = can reach (i,j) after exactly m moves
    vector<vector<vector<bool>>> dp(M+1, vector<vector<bool>>(N, vector<bool>(N, false)));
    
    dp[0][startX][startY] = true;
    
    // 8 directions: up, down, left, right, and 4 diagonals
    int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
    
    for(int m = 0; m < M; m++){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                if(!dp[m][i][j]) continue;
                
                // Try all 8 directions
                for(int dir = 0; dir < 8; dir++){
                    int x = i, y = j;
                    // Keep moving in this direction until we hit a wall or piece
                    while(true){
                        x += dx[dir];
                        y += dy[dir];
                        
                        // Check bounds
                        if(x < 0 || x >= N || y < 0 || y >= N) break;
                        
                        // Check if there's a white piece - can't move here
                        if(board[x][y] == 'W') break;
                        
                        // Can move to empty cell or black piece
                        dp[m+1][x][y] = true;
                        
                        // If black piece, stop here
                        if(board[x][y] == 'B') break;
                    }
                }
            }
        }
    }
    
    // Count all cells reachable after exactly M moves
    set<pair<int,int>> reachable;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(dp[M][i][j]){
                reachable.insert({i, j});
            }
        }
    }
    
    cout << reachable.size() << endl;
    
    return 0;
}
