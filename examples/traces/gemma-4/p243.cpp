#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

/**
 * Problem: Broken Chessboard (243)
 * The task is to reconstruct an N x N chessboard from K pieces given in a 20x20 grid.
 * Pieces are represented by uppercase letters and can be rotated by 0, 90, 180, or 270 degrees.
 * Each piece is a connected area. N is small (1 to 5), so we use a backtracking search.
 */

int N;
char grid[5][5];
bool used[26];

struct Piece {
    char label;
    vector<vector<pair<int, int>>> shapes;
};

vector<Piece> all_pieces;

// Recursive backtracking to fill the N x N grid
bool solve(int idx) {
    // If all cells have been considered
    if (idx == N * N) {
        // Check if all pieces have been used
        for (int i = 0; i < (int)all_pieces.size(); ++i) {
            if (!used[i]) return false;
        }
        return true;
    }

    int r = idx / N;
    int c = idx % N;

    // If current cell is already occupied, move to the next one
    if (grid[r][c] != '.') return solve(idx + 1);

    // Try to place an unused piece to cover the current empty cell (r, c)
    for (int i = 0; i < (int)all_pieces.size(); ++i) {
        if (used[i]) continue;

        used[i] = true;
        for (const auto& shape : all_pieces[i].shapes) {
            // Check if all cells of the current shape can be placed in the N x N grid
            bool ok = true;
            for (const auto& cell : shape) {
                int nr = r + cell.first;
                int nc = c + cell.second;
                if (nr < 0 || nr >= N || nc < 0 || nc >= N || grid[nr][nc] != '.') {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                // Place the piece
                for (const auto& cell : shape) {
                    grid[r + cell.first][c + cell.second] = all_pieces[i].label;
                }
                // Recursively try to fill the rest of the grid
                if (solve(idx + 1)) return true;
                // Backtrack: remove the piece
                for (const auto& cell : shape) {
                    grid[r + cell.first][c + cell.second] = '.';
                }
            }
        }
        used[i] = false;
    }

    return false;
}

int main() {
    // Fast input and output
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;
    vector<string> input(20);
    for (int i = 0; i < 20; ++i) cin >> input[i];

    // Step 1: Identify all pieces and their cell coordinates
    vector<pair<int, int>> piece_coords[26];
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (input[i][j] >= 'A' && input[i][j] <= 'Z') {
                piece_coords[input[i][j] - 'A'].push_back({i, j});
            }
        }
    }

    // Step 2: For each piece, pre-calculate all distinct rotated shapes
    for (int i = 0; i < 26; ++i) {
        if (piece_coords[i].empty()) continue;

        Piece p;
        p.label = (char)('A' + i);
        set<vector<pair<int, int>>> unique_shapes;

        for (int rot = 0; rot < 4; ++rot) {
            vector<pair<int, int>> current_cells = piece_coords[i];
            // Apply 90-degree rotation 'rot' times
            for (int r_rot = 0; r_rot < rot; ++r_rot) {
                for (auto& cell : current_cells) {
                    int old_r = cell.first;
                    int old_c = cell.second;
                    cell.first = old_c;
                    cell.second = -old_r;
                }
            }

            // Normalize the shape: the 'first' cell (smallest r, then smallest c) is (0, 0)
            int min_r = current_cells[0].first;
            for (auto& cell : current_cells) if (cell.first < min_r) min_r = cell.first;
            int min_c_at_min_r = 1000; // Larger than max grid index
            for (auto& cell : current_cells) {
                if (cell.first == min_r) {
                    if (cell.second < min_c_at_min_r) min_c_at_min_r = cell.second;
                }
            }

            for (auto& cell : current_cells) {
                cell.first -= min_r;
                cell.second -= min_c_at_min_r;
            }
            sort(current_cells.begin(), current_cells.end());
            unique_shapes.insert(current_cells);
        }
        for (auto& s : unique_shapes) p.shapes.push_back(s);
        all_pieces.push_back(p);
    }

    // Step 3: Initialize and solve the problem using backtracking
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            grid[i][j] = '.';
        }
    }

    // All pieces must be used, so if the grid is full, the pieces are consumed
    if (solve(0)) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << grid[i][j];
            }
            cout << "\n";
        }
    }

    return 0;
}

