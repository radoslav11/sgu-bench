#include<bits/stdc++.h>
using namespace std;

int n, m;
vector<string> board;
int kr, kc; // king position
vector<pair<int,int>> pieces; // positions of black pieces
int num_pieces;

// Check if a square (r,c) is attacked by a black piece at (pr,pc) of type t
bool isAttacked(int r, int c, int pr, int pc, char t) {
    if (t == 'K') { // Knight
        int dr = abs(r - pr);
        int dc = abs(c - pc);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
    else if (t == 'B') { // Bishop
        if (abs(r - pr) != abs(c - pc)) return false;
        int dr = (r > pr) ? 1 : (r < pr) ? -1 : 0;
        int dc = (c > pc) ? 1 : (c < pc) ? -1 : 0;
        int cr = pr + dr, cc = pc + dc;
        while (cr != r || cc != c) {
            if (board[cr][cc] != '.') return false;
            cr += dr;
            cc += dc;
        }
        return true;
    }
    else if (t == 'R') { // Rook
        if (r != pr && c != pc) return false;
        if (r == pr) {
            int start = min(c, pc);
            int end = max(c, pc);
            for (int i = start + 1; i < end; i++) {
                if (board[r][i] != '.') return false;
            }
            return true;
        } else {
            int start = min(r, pr);
            int end = max(r, pr);
            for (int i = start + 1; i < end; i++) {
                if (board[i][c] != '.') return false;
            }
            return true;
        }
    }
    return false;
}

bool isSafeToMove(int r, int c, int mask) {
    // Check if square (r,c) is attacked by any remaining black piece
    for (int i = 0; i < num_pieces; i++) {
        if (mask & (1 << i)) continue; // piece already captured
        if (isAttacked(r, c, pieces[i].first, pieces[i].second, board[pieces[i].first][pieces[i].second])) {
            return false;
        }
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m;
    board.resize(n);
    
    for (int i = 0; i < n; i++) {
        cin >> board[i];
        for (int j = 0; j < m; j++) {
            if (board[i][j] == '*') {
                kr = i;
                kc = j;
                board[i][j] = '.';
            } else if (board[i][j] != '.') {
                pieces.push_back({i, j});
            }
        }
    }
    
    num_pieces = pieces.size();
    if (num_pieces == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    // BFS
    map<tuple<int,int,int>, int> dist;
    queue<tuple<int,int,int>> q;
    
    int initial_mask = 0;
    q.push({kr, kc, initial_mask});
    dist[{kr, kc, initial_mask}] = 0;
    
    int full_mask = (1 << num_pieces) - 1;
    int ans = -1;
    
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    while (!q.empty()) {
        auto [r, c, mask] = q.front();
        q.pop();
        
        if (mask == full_mask) {
            ans = dist[{r, c, mask}];
            break;
        }
        
        for (int i = 0; i < 8; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            
            int new_mask = mask;
            // Check if we're capturing a piece
            for (int j = 0; j < num_pieces; j++) {
                if (!(mask & (1 << j)) && pieces[j].first == nr && pieces[j].second == nc) {
                    new_mask |= (1 << j);
                    break;
                }
            }
            
            if (!isSafeToMove(nr, nc, new_mask)) continue;
            
            if (dist.find({nr, nc, new_mask}) == dist.end()) {
                dist[{nr, nc, new_mask}] = dist[{r, c, mask}] + 1;
                q.push({nr, nc, new_mask});
            }
        }
    }
    
    cout << ans << endl;
    return 0;
}
