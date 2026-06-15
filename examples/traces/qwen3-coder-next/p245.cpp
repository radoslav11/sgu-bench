#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cctype>
#include <cstring>

using namespace std;

const int INF = 1e9;
const int MAXN = 305;

struct State {
    int r, c, piece;
    int score;
    State(int r, int c, int piece, int score) : r(r), c(c), piece(piece), score(score) {}
};

int N, M;
int bonus[6]; // pawn, rook, knight, bishop, queen, king
int penalty;

// Directions for each piece type
// 0: pawn, 1: rook, 2: knight, 3: bishop, 4: queen, 5: king
int dr[6][40], dc[6][40], moves_count[6];
char board[MAXN][MAXN];

// Directions for rook
int rook_dr[4] = {-1, 1, 0, 0};
int rook_dc[4] = {0, 0, -1, 1};

// Directions for bishop
int bishop_dr[4] = {-1, 1, -1, 1};
int bishop_dc[4] = {-1, -1, 1, 1};

// Directions for queen
int queen_dr[8] = {-1, 1, 0, 0, -1, 1, -1, 1};
int queen_dc[8] = {0, 0, -1, 1, -1, -1, 1, 1};

// Directions for knight
int knight_dr[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int knight_dc[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

// Directions for king
int king_dr[8] = {-1, 1, 0, 0, -1, 1, -1, 1};
int king_dc[8] = {0, 0, -1, 1, -1, -1, 1, 1};

// Directions for pawn (forward is up - decreasing row)
int pawn_dr[2] = {-1, -2}; // one step, two steps (only from start row)
int pawn_dc[2] = {0, 0};

// For checking if a piece attacks (r2, c2) from (r1, c1)
bool piece_attacks(int piece, int r1, int c1, int r2, int c2) {
    if (piece == 0) { // pawn
        // Pawn attacks diagonally forward (decreasing row direction)
        // Since board has decreasing row direction for pieces, pawn attacks: r1-1, c1-1 and r1-1, c1+1
        if (r1 - 1 == r2 && (c1 - 1 == c2 || c1 + 1 == c2))
            return true;
        return false;
    } else if (piece == 1) { // rook
        if (r1 == r2 || c1 == c2) {
            int dr = (r2 - r1 == 0) ? 0 : (r2 - r1 > 0 ? 1 : -1);
            int dc = (c2 - c1 == 0) ? 0 : (c2 - c1 > 0 ? 1 : -1);
            for (int i = r1 + dr, j = c1 + dc; ; i += dr, j += dc) {
                if (i == r2 && j == c2) return true;
                if (board[i][j] == '#' || board[i][j] == 'P' || board[i][j] == 'R' || 
                    board[i][j] == 'K' || board[i][j] == 'B' || board[i][j] == 'Q' || board[i][j] == 'M') {
                    return false;
                }
            }
        }
        return false;
    } else if (piece == 2) { // knight
        for (int i = 0; i < 8; i++) {
            if (r1 + knight_dr[i] == r2 && c1 + knight_dc[i] == c2)
                return true;
        }
        return false;
    } else if (piece == 3) { // bishop
        if (abs(r2 - r1) == abs(c2 - c1)) {
            int dr = (r2 > r1) ? 1 : -1;
            int dc = (c2 > c1) ? 1 : -1;
            for (int i = r1 + dr, j = c1 + dc; ; i += dr, j += dc) {
                if (i == r2 && j == c2) return true;
                if (board[i][j] == '#' || board[i][j] == 'P' || board[i][j] == 'R' || 
                    board[i][j] == 'K' || board[i][j] == 'B' || board[i][j] == 'Q' || board[i][j] == 'M') {
                    return false;
                }
            }
        }
        return false;
    } else if (piece == 4) { // queen
        if (r1 == r2 || c1 == c2 || abs(r2 - r1) == abs(c2 - c1)) {
            if (r1 == r2) {
                int dc = (c2 > c1) ? 1 : -1;
                for (int j = c1 + dc; ; j += dc) {
                    if (j == c2) return true;
                    if (board[r1][j] == '#' || board[r1][j] == 'P' || board[r1][j] == 'R' || 
                        board[r1][j] == 'K' || board[r1][j] == 'B' || board[r1][j] == 'Q' || board[r1][j] == 'M') {
                        return false;
                    }
                }
            } else if (c1 == c2) {
                int dr = (r2 > r1) ? 1 : -1;
                for (int i = r1 + dr; ; i += dr) {
                    if (i == r2) return true;
                    if (board[i][c1] == '#' || board[i][c1] == 'P' || board[i][c1] == 'R' || 
                        board[i][c1] == 'K' || board[i][c1] == 'B' || board[i][c1] == 'Q' || board[i][c1] == 'M') {
                        return false;
                    }
                }
            } else {
                int dr = (r2 > r1) ? 1 : -1;
                int dc = (c2 > c1) ? 1 : -1;
                for (int i = r1 + dr, j = c1 + dc; ; i += dr, j += dc) {
                    if (i == r2 && j == c2) return true;
                    if (board[i][j] == '#' || board[i][j] == 'P' || board[i][j] == 'R' || 
                        board[i][j] == 'K' || board[i][j] == 'B' || board[i][j] == 'Q' || board[i][j] == 'M') {
                        return false;
                    }
                }
            }
        }
        return false;
    } else if (piece == 5) { // king
        for (int i = 0; i < 8; i++) {
            if (r1 + king_dr[i] == r2 && c1 + king_dc[i] == c2)
                return true;
        }
        return false;
    }
    return false;
}

bool is_under_attack(int r, int c, int my_piece) {
    // Check all colorless pieces (P, R, K, B, Q, M)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] == 'P' || board[i][j] == 'R' || board[i][j] == 'K' || 
                board[i][j] == 'B' || board[i][j] == 'Q' || board[i][j] == 'M') {
                int piece_type;
                if (board[i][j] == 'P') piece_type = 0;
                else if (board[i][j] == 'R') piece_type = 1;
                else if (board[i][j] == 'K') piece_type = 2;
                else if (board[i][j] == 'B') piece_type = 3;
                else if (board[i][j] == 'Q') piece_type = 4;
                else if (board[i][j] == 'M') piece_type = 5;
                if (piece_attacks(piece_type, i, j, r, c)) {
                    // Special case for pawn: it attacks diagonally but cannot move forward in this problem
                    // Also note: pawns attack only forward (decreasing row), but since the problem says
                    // "colorless chess pieces are oriented in direction of decreasing line numbers", 
                    // and we are moving in opposite direction, we need to check correctly.
                    if (piece_type == 0) {
                        // Pawn at (i, j) attacks (i-1, j-1) and (i-1, j+1) - same as our definition above
                        if (piece_attacks(piece_type, i, j, r, c)) return true;
                    } else {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// BFS/Dijkstra-like approach with states (r, c, piece_type)
int solve() {
    int start_r, start_c;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] == '@') {
                start_r = i;
                start_c = j;
            }
        }
    }

    // dist[r][c][piece_type] = maximum score achievable at (r, c) with piece type
    vector<vector<vector<int>>> dist(N, vector<vector<int>>(M, vector<int>(6, -INF)));
    // Using priority queue for Dijkstra-like max-heap (by score)
    priority_queue<tuple<int, int, int, int>> pq; // (-score, r, c, piece) for min-heap behavior with negative

    // Try all possible starting pieces (except queen)
    for (int p = 0; p < 6; p++) {
        if (p == 4) continue; // queen not allowed
        // Check if starting position is safe
        if (!is_under_attack(start_r, start_c, p)) {
            dist[start_r][start_c][p] = 0;
            pq.push({0, start_r, start_c, p});
        }
    }

    int best_score = 0;

    while (!pq.empty()) {
        auto [neg_score, r, c, piece] = pq.top();
        pq.pop();
        int score = -neg_score;

        if (score != dist[r][c][piece]) continue;

        best_score = max(best_score, score);

        // Try moving to adjacent cells based on piece type
        // For sliding pieces (rook, bishop, queen) we can move multiple steps
        // For knight and king and pawn, it's specific patterns

        vector<pair<int, int>> moves;
        
        if (piece == 0) { // pawn
            // Pawns move forward (decreasing row) - one or two steps from start
            // One step forward
            int nr1 = r - 1;
            int nc1 = c;
            if (nr1 >= 0 && board[nr1][nc1] != '#' && board[nr1][nc1] != 'P' && 
                board[nr1][nc1] != 'R' && board[nr1][nc1] != 'K' && 
                board[nr1][nc1] != 'B' && board[nr1][nc1] != 'Q' && 
                board[nr1][nc1] != 'M') {
                moves.push_back({nr1, nc1});
            }
            // Two steps from starting position (only if it's the second row from top? The problem says "can not make move from the second line to the fourth"
            // We'll assume two steps only if we are at row 1 (0-indexed) and row 0 is empty
            // Actually, standard chess: pawns can move two squares only from starting position
            // Since the board orientation: colorless pawns move in direction of decreasing row numbers
            // For our pawn, moving in opposite direction (increasing row numbers) — NO! Wait.
            // Re-reading: "colorless chess pieces are oriented in direction of decreasing line numbers, and you are oriented in the opposite direction"
            // So colorless pawns move upward (row decreases), we (player) are oriented downward (row increases).
            // But pawn moves: pawn moves in direction of increasing row numbers (downwards)
            // Pawns capture diagonally forward: to (r+1, c-1) and (r+1, c+1) (since moving down)
            // However, the problem says: "it is considered (it is important for pawns only) that colorless chess pieces are oriented in direction of decreasing line numbers"
            // So pawns on board move UP (row decreases), but we are oriented opposite -> we move DOWN (row increases).
            // But then the pawn move for us would be DOWN, and we attack diagonally DOWN: (r+1, c-1) and (r+1, c+1)
            // However, the note: "if you choose a pawn you can not change your chess piece type when moving to the last line"
            // and "can not make move from the second line to the fourth" suggests:
            // Standard chess pawn moves: from row r to r-1 (or r+1), and two steps from start (only if rows 1 to 3 or 6 to 4)
            // Since we move in direction of increasing row numbers, our pawn moves from r to r+1 (one step), 
            // and two steps from row r to r+2 (if r+1 is empty)
            
            // Correction: The problem says: "colorless chess pieces are oriented in direction of decreasing line numbers", 
            // meaning: pawns on board (colorless) move UP (row index decreases), we are oriented opposite -> we move DOWN (row index increases).
            // So we need to move our pawn in direction of increasing row numbers.
            // But the sample input: 
            // 4 5
            // ..@..
            // .K.K.
            // .Q...
            // .....
            // @ is at (0,2), then below at row 1: .K.K., row 2: .Q..., row 3: .....
            // So increasing row is down.
            
            // For pawns: 
            // Standard move: from (r, c) to (r+1, c) for one step, (r+2, c) for two steps (only if on starting row, which would be row 1 (0-indexed) in 4-row board)
            // But problem says: "can not make move from the second line to the fourth" — meaning if we're on second line (row index 1), we cannot jump to fourth (row index 3).
            // So starting position for two-step move is row 1 (0-indexed).
            
            // Let's fix: 
            // Two steps: only if r == 1 and r+1 < N and board[r+1][c] is empty and board[r+2][c] is empty
            if (r == 1 && r + 2 < N && board[r+1][c] != '#' && board[r+1][c] != 'P' && 
                board[r+1][c] != 'R' && board[r+1][c] != 'K' && 
                board[r+1][c] != 'B' && board[r+1][c] != 'Q' && 
                board[r+1][c] != 'M' &&
                board[r+2][c] != '#' && board[r+2][c] != 'P' && 
                board[r+2][c] != 'R' && board[r+2][c] != 'K' && 
                board[r+2][c] != 'B' && board[r+2][c] != 'Q' && 
                board[r+2][c] != 'M') {
                moves.push_back({r+2, c});
            }
            // Also capture moves: diagonally down (r+1, c-1) and (r+1, c+1)
            // But capturing is not standard move — actually in chess, pawn moves forward but captures diagonally
            // However, in this problem, moving onto a cell with a piece means capturing it, which is allowed only if it's not a sculpture
            // But note: the problem says: "If you want you can leave training grounds in the beginning of any turn (not very patriotic) and do not lose any score points."
            // and "after capturing colorless chess piece of some type your score is increased by a capturing bonus"
            // So capturing is done when moving onto a cell with a piece.
            // But pawns capture diagonally forward — which for us is (r+1, c-1) and (r+1, c+1)
            // So we need to consider capture moves too, but they are not "move", they're actions — but in our state, we move and land on a cell
            // So the move for pawn includes: forward move (one or two steps) and capture moves (diagonally down)
            // Let's reconsider: in chess, pawns have different move and capture patterns.
            // For our pawn, moving forward: (r+1, c), two steps: (r+2, c) (if on starting row and path clear)
            // Capturing: (r+1, c-1) and (r+1, c+1) — but only if occupied by enemy piece
            
            // So we need to consider both move and capture for pawns.
            // In our BFS, when we move to a cell, if it's occupied, we capture (and get bonus).
            // So let's extend moves to include:
            // - (r+1, c) — forward move
            // - (r+1, c-1), (r+1, c+1) — capture moves
            // - (r+2, c) — forward two steps (only if r==1 and both cells empty)
            
            // Add forward move
            if (r + 1 < N && board[r+1][c] != '#' && board[r+1][c] != 'P' && 
                board[r+1][c] != 'R' && board[r+1][c] != 'K' && 
                board[r+1][c] != 'B' && board[r+1][c] != 'Q' && 
                board[r+1][c] != 'M') {
                moves.push_back({r+1, c});
            }
            // Add capture moves
            if (r + 1 < N && c - 1 >= 0 && board[r+1][c-1] != '.' && 
                board[r+1][c-1] != '#') {
                moves.push_back({r+1, c-1});
            }
            if (r + 1 < N && c + 1 < M && board[r+1][c+1] != '.' && 
                board[r+1][c+1] != '#') {
                moves.push_back({r+1, c+1});
            }
        } else if (piece == 1) { // rook
            for (int d = 0; d < 4; d++) {
                int nr = r + rook_dr[d];
                int nc = c + rook_dc[d];
                while (nr >= 0 && nr < N && nc >= 0 && nc < M) {
                    if (board[nr][nc] == '#' || board[nr][nc] == 'P' || 
                        board[nr][nc] == 'R' || board[nr][nc] == 'K' || 
                        board[nr][nc] == 'B' || board[nr][nc] == 'Q' || 
                        board[nr][nc] == 'M') {
                        if (nr == r + rook_dr[d] && nc == c + rook_dc[d]) break; // can't capture obstacle on first step
                        break;
                    }
                    moves.push_back({nr, nc});
                    if (board[nr][nc] == '.') {
                        nr += rook_dr[d];
                        nc += rook_dc[d];
                    } else {
                        break;
                    }
                }
            }
        } else if (piece == 2) { // knight
            for (int d = 0; d < 8; d++) {
                int nr = r + knight_dr[d];
                int nc = c + knight_dc[d];
                if (nr >= 0 && nr < N && nc >= 0 && nc < M && 
                    board[nr][nc] != '#') {
                    moves.push_back({nr, nc});
                }
            }
        } else if (piece == 3) { // bishop
            for (int d = 0; d < 4; d++) {
                int nr = r + bishop_dr[d];
                int nc = c + bishop_dc[d];
                while (nr >= 0 && nr < N && nc >= 0 && nc < M) {
                    if (board[nr][nc] == '#' || board[nr][nc] == 'P' || 
                        board[nr][nc] == 'R' || board[nr][nc] == 'K' || 
                        board[nr][nc] == 'B' || board[nr][nc] == 'Q' || 
                        board[nr][nc] == 'M') {
                        if (nr == r + bishop_dr[d] && nc == c + bishop_dc[d]) break;
                        break;
                    }
                    moves.push_back({nr, nc});
                    if (board[nr][nc] == '.') {
                        nr += bishop_dr[d];
                        nc += bishop_dc[d];
                    } else {
                        break;
                    }
                }
            }
        } else if (piece == 4) { // queen
            for (int d = 0; d < 8; d++) {
                int nr = r + queen_dr[d];
                int nc = c + queen_dc[d];
                while (nr >= 0 && nr < N && nc >= 0 && nc < M) {
                    if (board[nr][nc] == '#' || board[nr][nc] == 'P' || 
                        board[nr][nc] == 'R' || board[nr][nc] == 'K' || 
                        board[nr][nc] == 'B' || board[nr][nc] == 'Q' || 
                        board[nr][nc] == 'M') {
                        if (nr == r + queen_dr[d] && nc == c + queen_dc[d]) break;
                        break;
                    }
                    moves.push_back({nr, nc});
                    if (board[nr][nc] == '.') {
                        nr += queen_dr[d];
                        nc += queen_dc[d];
                    } else {
                        break;
                    }
                }
            }
        } else if (piece == 5) { // king
            for (int d = 0; d < 8; d++) {
                int nr = r + king_dr[d];
                int nc = c + king_dc[d];
                if (nr >= 0 && nr < N && nc >= 0 && nc < M && 
                    board[nr][nc] != '#') {
                    moves.push_back({nr, nc});
                }
            }
        }
        
        for (auto [nr, nc] : moves) {
            // If we land on a piece (non-empty, non-#), we capture it and get bonus
            int new_score = score;
            char cell = board[nr][nc];
            if (cell != '.') {
                if (cell == 'P') new_score += bonus[0];
                else if (cell == 'R') new_score += bonus[1];
                else if (cell == 'K') new_score += bonus[2];
                else if (cell == 'B') new_score += bonus[3];
                else if (cell == 'M') new_score += bonus[5]; // king
                // Note: queen not allowed as our piece but can be captured (bonus[4])
                // But we are not queen, so we can't be attacked by queen? Actually we can be attacked by queen
                // But for capturing: if we land on queen, we can capture it only if we have a piece that can capture queen?
                // No — in chess, any piece can capture any piece, regardless of type
                // So if cell == 'Q', we capture it and get bonus[4]
                else if (cell == 'Q') new_score += bonus[4];
            }
            
            // Check if after moving, we are under attack
            if (is_under_attack(nr, nc, piece)) {
                // We lose, score = new_score - penalty, but we can choose to leave (which is 0 for this path)
                // Actually, the problem: "If after the turn you are under attack of some colorless chess piece, you lose (the training is over) and your score points is decreased by some penalty value."
                // So new_score -= penalty;
                // But then the game ends, so we don't continue from this state
                // So we can record this as a terminal state
                best_score = max(best_score, new_score - penalty);
                continue;
            }
            
            // If not under attack, and we have a better score for this state
            if (new_score > dist[nr][nc][piece]) {
                dist[nr][nc][piece] = new_score;
                pq.push({-new_score, nr, nc, piece});
            }
        }
    }

    return best_score;
}

int main() {
    cin >> N >> M;
    for (int i = 0; i < 6; i++) {
        cin >> bonus[i];
    }
    cin >> penalty;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> board[i][j];
        }
    }
    
    cout << solve() << endl;
    
    return 0;
}
