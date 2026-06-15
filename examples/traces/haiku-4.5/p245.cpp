#include<bits/stdc++.h>
using namespace std;

int N, M;
vector<string> board;
int bonus[6]; // pawn, rook, knight, bishop, queen, king
int penalty;
int si, sj; // start position

// Direction vectors for each piece
int pawnDi[] = {-1};
int pawnDj[] = {0};

int rookDi[] = {-1, 1, 0, 0};
int rookDj[] = {0, 0, -1, 1};

int knightDi[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int knightDj[] = {-1, 1, -2, 2, -2, 2, -1, 1};

int bishopDi[] = {-1, -1, 1, 1};
int bishopDj[] = {-1, 1, -1, 1};

int kingDi[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int kingDj[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool isUnderAttack(int i, int j, set<pair<int,int>>& captured) {
    for(int pi = 0; pi < N; pi++) {
        for(int pj = 0; pj < M; pj++) {
            if(captured.count({pi, pj})) continue;
            char c = board[pi][pj];
            if(c == 'P' || c == 'R' || c == 'K' || c == 'B' || c == 'Q') {
                // Check if this piece attacks position (i,j)
                if(c == 'P') {
                    if(pi-1 == i && (pj-1 == j || pj+1 == j)) return true;
                } else if(c == 'R') {
                    if(pi == i || pj == j) return true;
                } else if(c == 'K') {
                    if(abs(pi-i) <= 2 && abs(pj-j) <= 2 && (abs(pi-i) + abs(pj-j) <= 4)) return true;
                } else if(c == 'B') {
                    if(abs(pi-i) == abs(pj-j)) return true;
                } else if(c == 'Q') {
                    if(pi == i || pj == j || abs(pi-i) == abs(pj-j)) return true;
                }
            }
        }
    }
    return false;
}

int solve(int piece) {
    // piece: 0=pawn, 1=rook, 2=knight, 3=bishop, 4=king
    map<tuple<int,int,set<pair<int,int>>>, int> memo;
    queue<tuple<int,int,set<pair<int,int>>, int>> q;
    set<pair<int,int>> empty;
    q.push({si, sj, empty, 0});
    
    int maxScore = 0;
    set<tuple<int,int,set<pair<int,int>>>> visited;
    
    while(!q.empty()) {
        auto [i, j, captured, score] = q.front();
        q.pop();
        
        auto state = make_tuple(i, j, captured);
        if(visited.count(state)) continue;
        visited.insert(state);
        
        maxScore = max(maxScore, score);
        
        // Try to move
        vector<pair<int,int>> moves;
        if(piece == 0) { // pawn
            if(i > 0 && board[i-1][j] == '.') moves.push_back({i-1, j});
            if(i > 0 && (board[i-1][j] == 'P' || board[i-1][j] == 'R' || board[i-1][j] == 'K' || 
                         board[i-1][j] == 'B' || board[i-1][j] == 'Q') && !captured.count({i-1, j})) 
                moves.push_back({i-1, j});
        } else if(piece == 1) { // rook
            for(int d = 0; d < 4; d++)
                for(int k = 1; k < max(N,M); k++) {
                    int ni = i + k*rookDi[d], nj = j + k*rookDj[d];
                    if(ni < 0 || ni >= N || nj < 0 || nj >= M || board[ni][nj] == '#') break;
                    if(board[ni][nj] == '.' || (captured.count({ni,nj}) == 0 && board[ni][nj] != '#' && board[ni][nj] != '@'))
                        moves.push_back({ni, nj});
                    if(board[ni][nj] != '.') break;
                }
        } else if(piece == 2) { // knight
            for(int d = 0; d < 8; d++) {
                int ni = i + knightDi[d], nj = j + knightDj[d];
                if(ni >= 0 && ni < N && nj >= 0 && nj < M && board[ni][nj] != '#')
                    moves.push_back({ni, nj});
            }
        } else if(piece == 3) { // bishop
            for(int d = 0; d < 4; d++)
                for(int k = 1; k < max(N,M); k++) {
                    int ni = i + k*bishopDi[d], nj = j + k*bishopDj[d];
                    if(ni < 0 || ni >= N || nj < 0 || nj >= M || board[ni][nj] == '#') break;
                    if(board[ni][nj] == '.' || (captured.count({ni,nj}) == 0 && board[ni][nj] != '#'))
                        moves.push_back({ni, nj});
                    if(board[ni][nj] != '.') break;
                }
        } else if(piece == 4) { // king
            for(int d = 0; d < 8; d++) {
                int ni = i + kingDi[d], nj = j + kingDj[d];
                if(ni >= 0 && ni < N && nj >= 0 && nj < M && board[ni][nj] != '#')
                    moves.push_back({ni, nj});
            }
        }
        
        for(auto [ni, nj] : moves) {
            auto newCaptured = captured;
            int newScore = score;
            if(board[ni][nj] != '.' && board[ni][nj] != '@' && !captured.count({ni, nj})) {
                newCaptured.insert({ni, nj});
                if(board[ni][nj] == 'P') newScore += bonus[0];
                else if(board[ni][nj] == 'R') newScore += bonus[1];
                else if(board[ni][nj] == 'K') newScore += bonus[2];
                else if(board[ni][nj] == 'B') newScore += bonus[3];
                else if(board[ni][nj] == 'Q') newScore += bonus[4];
            }
            
            if(!isUnderAttack(ni, nj, newCaptured)) {
                q.push({ni, nj, newCaptured, newScore});
            }
        }
    }
    
    return maxScore;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    for(int i = 0; i < 6; i++) cin >> bonus[i];
    cin >> penalty;
    
    board.resize(N);
    for(int i = 0; i < N; i++) {
        cin >> board[i];
        for(int j = 0; j < M; j++) {
            if(board[i][j] == '@') {
                si = i; sj = j;
            }
        }
    }
    
    int ans = 0;
    for(int piece = 0; piece < 5; piece++) {
        ans = max(ans, solve(piece));
    }
    
    cout << ans << endl;
    
    return 0;
}
