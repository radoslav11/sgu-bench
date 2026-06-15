#include<bits/stdc++.h>
using namespace std;

int N;
vector<pair<int,int>> pieces[26];
char result[5][5];
bool used[26];

vector<pair<int,int>> normalize(vector<pair<int,int>> piece) {
    if(piece.empty()) return piece;
    int minr = INT_MAX, minc = INT_MAX;
    for(auto [r,c] : piece) {
        minr = min(minr, r);
        minc = min(minc, c);
    }
    for(auto& [r,c] : piece) {
        r -= minr;
        c -= minc;
    }
    return piece;
}

vector<pair<int,int>> rotate90(vector<pair<int,int>> piece) {
    vector<pair<int,int>> rotated;
    for(auto [r,c] : piece) {
        rotated.push_back({c, -r});
    }
    return normalize(rotated);
}

bool canPlace(char letter, vector<pair<int,int>>& piece, int sr, int sc) {
    for(auto [dr, dc] : piece) {
        int nr = sr + dr;
        int nc = sc + dc;
        if(nr < 0 || nr >= N || nc < 0 || nc >= N) return false;
        if(result[nr][nc] != '.') return false;
    }
    return true;
}

void place(char letter, vector<pair<int,int>>& piece, int sr, int sc) {
    for(auto [dr, dc] : piece) {
        result[sr+dr][sc+dc] = letter;
    }
}

void unplace(vector<pair<int,int>>& piece, int sr, int sc) {
    for(auto [dr, dc] : piece) {
        result[sr+dr][sc+dc] = '.';
    }
}

bool solve(int pieceIdx) {
    if(pieceIdx == 26) {
        // Check if board is full
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(result[i][j] == '.') return false;
            }
        }
        return true;
    }
    
    if(pieces[pieceIdx].empty()) {
        return solve(pieceIdx + 1);
    }
    
    char letter = 'A' + pieceIdx;
    vector<pair<int,int>> current = pieces[pieceIdx];
    
    // Try all 4 rotations
    for(int rot = 0; rot < 4; rot++) {
        // Try all positions
        for(int r = 0; r < N; r++) {
            for(int c = 0; c < N; c++) {
                if(canPlace(letter, current, r, c)) {
                    place(letter, current, r, c);
                    if(solve(pieceIdx + 1)) return true;
                    unplace(current, r, c);
                }
            }
        }
        current = rotate90(current);
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    cin.ignore();
    
    vector<string> grid(20);
    for(int i = 0; i < 20; i++) {
        getline(cin, grid[i]);
    }
    
    // Extract pieces
    bool visited[20][20] = {};
    
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            if(!visited[i][j] && grid[i][j] != '.') {
                char letter = grid[i][j];
                vector<pair<int,int>> piece;
                queue<pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                
                while(!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();
                    piece.push_back({r, c});
                    
                    int dr[] = {-1, 1, 0, 0};
                    int dc[] = {0, 0, -1, 1};
                    for(int d = 0; d < 4; d++) {
                        int nr = r + dr[d];
                        int nc = c + dc[d];
                        if(nr >= 0 && nr < 20 && nc >= 0 && nc < 20 && 
                           !visited[nr][nc] && grid[nr][nc] == letter) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                        }
                    }
                }
                
                pieces[letter - 'A'] = normalize(piece);
            }
        }
    }
    
    // Initialize result board
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            result[i][j] = '.';
        }
    }
    
    solve(0);
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << result[i][j];
        }
        cout << '\n';
    }
    
    return 0;
}
