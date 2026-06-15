#include<bits/stdc++.h>
using namespace std;

int N, M, K;
vector<int> lengths;
vector<vector<char>> grid;

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < M;
}

bool solve(int pos) {
    // Find next empty cell
    while(pos < N * M) {
        int r = pos / M;
        int c = pos % M;
        if(grid[r][c] == '.') break;
        pos++;
    }
    
    if(pos == N * M) {
        // All cells filled
        return true;
    }
    
    int r = pos / M;
    int c = pos % M;
    char brick = 'a' + (pos % 26); // Use different letter for each brick
    
    // Try placing brick horizontally
    bool canPlaceHorizontal = true;
    for(int i = 0; i < K; i++) {
        int len = lengths[i];
        if(c + len <= M) {
            // Check if all cells are empty
            canPlaceHorizontal = true;
            for(int j = 0; j < len; j++) {
                if(grid[r][c+j] != '.') {
                    canPlaceHorizontal = false;
                    break;
                }
            }
            
            if(canPlaceHorizontal) {
                // Place brick
                for(int j = 0; j < len; j++) {
                    grid[r][c+j] = brick;
                }
                
                if(solve(pos + 1)) return true;
                
                // Remove brick
                for(int j = 0; j < len; j++) {
                    grid[r][c+j] = '.';
                }
            }
        }
    }
    
    // Try placing brick vertically
    for(int i = 0; i < K; i++) {
        int len = lengths[i];
        if(r + len <= N) {
            // Check if all cells are empty
            bool canPlaceVertical = true;
            for(int j = 0; j < len; j++) {
                if(grid[r+j][c] != '.') {
                    canPlaceVertical = false;
                    break;
                }
            }
            
            if(canPlaceVertical) {
                // Place brick
                for(int j = 0; j < len; j++) {
                    grid[r+j][c] = brick;
                }
                
                if(solve(pos + 1)) return true;
                
                // Remove brick
                for(int j = 0; j < len; j++) {
                    grid[r+j][c] = '.';
                }
            }
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> K;
    lengths.resize(K);
    
    for(int i = 0; i < K; i++) {
        cin >> lengths[i];
    }
    
    grid.assign(N, vector<char>(M, '.'));
    
    if(solve(0)) {
        cout << "YES\n";
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                cout << grid[i][j];
            }
            cout << "\n";
        }
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
