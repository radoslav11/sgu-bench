#include<bits/stdc++.h>
using namespace std;

int N;
vector<vector<int>> board;
int dx[] = {2, 2, 1, 1, -1, -1, -2, -2};
int dy[] = {1, -1, 2, -2, 2, -2, 1, -1};

int countOnward(int x, int y) {
    int count = 0;
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(nx >= 0 && nx < N && ny >= 0 && ny < N && board[nx][ny] == 0) {
            count++;
        }
    }
    return count;
}

bool solve(int x, int y, int move) {
    board[x][y] = move;
    
    if(move == N * N) {
        return true;
    }
    
    // Warnsdorff's heuristic: try moves in order of fewest onward moves
    vector<pair<int, pair<int, int>>> nextMoves;
    
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(nx >= 0 && nx < N && ny >= 0 && ny < N && board[nx][ny] == 0) {
            int onward = countOnward(nx, ny);
            nextMoves.push_back({onward, {nx, ny}});
        }
    }
    
    sort(nextMoves.begin(), nextMoves.end());
    
    for(auto& p : nextMoves) {
        int nx = p.second.first;
        int ny = p.second.second;
        if(solve(nx, ny, move + 1)) {
            return true;
        }
    }
    
    board[x][y] = 0;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    
    // Special cases
    if(N == 2 || N == 3) {
        cout << "No solution.\n";
        return 0;
    }
    
    board.assign(N, vector<int>(N, 0));
    
    // Try starting from (0,0)
    if(solve(0, 0, 1)) {
        cout << "There is solution:\n";
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(j > 0) cout << " ";
                cout << board[i][j];
            }
            cout << "\n";
        }
    } else {
        cout << "No solution.\n";
    }
    
    return 0;
}
