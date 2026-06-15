#include<bits/stdc++.h>
using namespace std;

int M, N;
vector<string> grid;
int minPieces;

bool isValid(int row, int col) {
    return row >= 0 && row < M && col >= 0 && col < N;
}

void solve(int pos, int pieces, vector<vector<bool>>& covered) {
    // Find next uncovered empty cell
    while(pos < M * N) {
        int r = pos / N;
        int c = pos % N;
        if(grid[r][c] == '.' && !covered[r][c]) break;
        pos++;
    }
    
    // If all cells processed
    if(pos == M * N) {
        // Check if any two adjacent empty cells exist
        bool valid = true;
        for(int r = 0; r < M && valid; r++) {
            for(int c = 0; c < N && valid; c++) {
                if(grid[r][c] == '.' && !covered[r][c]) {
                    // Check neighbors
                    if((isValid(r+1, c) && grid[r+1][c] == '.' && !covered[r+1][c]) ||
                       (isValid(r, c+1) && grid[r][c+1] == '.' && !covered[r][c+1])) {
                        valid = false;
                    }
                }
            }
        }
        if(valid) {
            minPieces = min(minPieces, pieces);
        }
        return;
    }
    
    int r = pos / N;
    int c = pos % N;
    
    // Try placing horizontal domino
    if(isValid(r, c+1) && grid[r][c+1] == '.' && !covered[r][c+1]) {
        covered[r][c] = covered[r][c+1] = true;
        solve(pos + 1, pieces + 1, covered);
        covered[r][c] = covered[r][c+1] = false;
    }
    
    // Try placing vertical domino
    if(isValid(r+1, c) && grid[r+1][c] == '.' && !covered[r+1][c]) {
        covered[r][c] = covered[r+1][c] = true;
        solve(pos + 1, pieces + 1, covered);
        covered[r][c] = covered[r+1][c] = false;
    }
    
    // Try not covering this cell (will be blocked by neighbors)
    solve(pos + 1, pieces, covered);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> M >> N;
    grid.resize(M);
    for(int i = 0; i < M; i++) {
        cin >> grid[i];
    }
    
    minPieces = INT_MAX;
    vector<vector<bool>> covered(M, vector<bool>(N, false));
    
    // Mark candle positions as covered
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            if(grid[i][j] == '*') {
                covered[i][j] = true;
            }
        }
    }
    
    solve(0, 0, covered);
    
    cout << minPieces << endl;
    
    return 0;
}
