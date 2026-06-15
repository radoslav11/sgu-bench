#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NUM_CELLS = 31;

// Given board layout and adjacency for marked cells
const vector<vector<int>> rows = {
    {0, 1},                                    // row 1: cells 1 2
    {2, 3, 4, 5, 6},                          // row 2: cells 3 4 5 6 7
    {7, 8, 9, 10, 11, 12},                   // row 3: cells 8 9 10 11 12 13
    {13, 14, 15, 16, 17},                   // row 4: cells 14 15 16 17 18
    {18, 19, 20, 21, 22, 23},              // row 5: cells 19 20 21 22 23 24
    {24, 25, 26, 27, 28},                  // row 6: cells 25 26 27 28 29
    {29, 30}                                // row 7: cells 30 31
};

const vector<vector<int>> diag_slash = {   // "/" diagonals (lower-left to upper-right)
    {2, 7},                                  // 3 8
    {0, 3, 8, 13, 18},                    // 1 4 9 14 19
    {1, 4, 9, 14, 19, 24},               // 2 5 10 15 20 25
    {5, 10, 15, 20, 25},                 // 6 11 16 21 26
    {6, 11, 16, 21, 26, 29},            // 7 12 17 22 27 30
    {12, 17, 22, 27, 30},               // 13 18 23 28 31
    {23, 28}                            // 24 29
};

const vector<vector<int>> diag_backslash = {  // "\" diagonals (upper-left to lower-right)
    {6, 12},                              // 7 13
    {1, 5, 11, 17, 23},                // 2 6 12 18 24
    {0, 4, 10, 16, 22, 28},           // 1 5 11 17 23 29
    {3, 9, 15, 21, 27},              // 4 10 16 22 28
    {2, 8, 14, 20, 26, 30},         // 3 9 15 21 27 31
    {7, 13, 19, 25, 31},            // 8 14 20 26 30
    {18, 24}                        // 19 25
};

// Marked cells: 5, 9, 12, 16, 20, 23, 27 (1-indexed) -> indices 4,8,11,15,19,22,26 (0-indexed)
const vector<int> marked_indices = {4, 8, 11, 15, 19, 22, 26};

// Precomputed neighbors for each marked cell (including itself)
const vector<vector<int>> marked_groups = {
    {0, 1, 3, 4, 5, 9, 10},      // for cell 5 (index 4)
    {2, 3, 7, 8, 9, 13, 14},     // for cell 9 (index 8)
    {5, 6, 10, 11, 12, 16, 17},  // for cell 12 (index 11)
    {9, 10, 14, 15, 16, 20, 21}, // for cell 16 (index 15)
    {13, 14, 18, 19, 20, 24, 25}, // for cell 20 (index 19)
    {16, 17, 21, 22, 23, 27, 28}, // for cell 23 (index 22)
    {20, 21, 25, 26, 27, 29, 30}  // for cell 27 (index 26)
};

// Adjacency list for all cells (for constraint propagation)
// But marked groups already include self and all neighbors
// So we'll just use marked_groups for the extra constraints

int K;
ll N;

// Current board state (0-indexed)
vector<int> board(NUM_CELLS, 0);

// Precompute which constraints each cell belongs to
// We'll use boolean arrays to check validity quickly
// Instead, we'll use arrays: used_in_row, used_in_diag_slash, used_in_diag_backslash
// But easier: precompute row/diag indices per cell

vector<vector<int>> cell_to_rows(NUM_CELLS);
vector<vector<int>> cell_to_slash(NUM_CELLS);
vector<vector<int>> cell_to_backslash(NUM_CELLS);

void build_constraint_indices() {
    for (int i = 0; i < (int)rows.size(); ++i) {
        for (int idx : rows[i]) {
            cell_to_rows[idx].push_back(i);
        }
    }
    for (int i = 0; i < (int)diag_slash.size(); ++i) {
        for (int idx : diag_slash[i]) {
            cell_to_slash[idx].push_back(i);
        }
    }
    for (int i = 0; i < (int)diag_backslash.size(); ++i) {
        for (int idx : diag_backslash[i]) {
            cell_to_backslash[idx].push_back(i);
        }
    }
}

// For each constraint type, track used numbers
vector<vector<bool>> used_in_row;
vector<vector<bool>> used_in_slash;
vector<vector<bool>> used_in_backslash;

// For marked groups, we can check by iterating
// But to make it fast, we can maintain a separate count or use bitsets.
// Since K <= 31, we can use bitsets or arrays of bools for each group

// Alternatively, when checking a cell assignment, we can:
// 1. Check row/diag constraints using the used_in_* arrays
// 2. For each marked group that contains this cell, check that all 7 values are distinct

// But checking a marked group on each assignment would be expensive (up to 7 groups * 7 checks)
// Instead, maintain for each marked group a count of used numbers and a flag if conflict

// However, since we are doing backtracking and only one cell changes at a time,
// we can update the marked groups efficiently.

// Let's precompute which marked groups each cell belongs to
vector<vector<int>> cell_to_marked_groups(NUM_CELLS);
for (int i = 0; i < (int)marked_groups.size(); ++i) {
    for (int idx : marked_groups[i]) {
        cell_to_marked_groups[idx].push_back(i);
    }
}

// For each marked group, maintain a frequency array (size K+1) or just a bitset
// Since K <= 31, we can use a vector of int arrays or vector<vector<int>> counts
vector<vector<int>> group_counts; // group_counts[group_id][num] = count
vector<bool> group_valid;         // group_counts[group_id] all <= 1?

void init_marked_groups() {
    group_counts.resize(7, vector<int>(K + 1, 0));
    group_valid.resize(7, true);
}

void update_marked_group(int group_id, int num, int delta) {
    group_counts[group_id][num] += delta;
    if (group_counts[group_id][num] == 2) {
        group_valid[group_id] = false;
    }
    if (group_counts[group_id][num] == 1 && delta == -1) {
        // If it was 2 before, now it's 1, so it might be valid again
        // But better to recompute group_valid[group_id]
    }
}

bool check_group_valid(int group_id) {
    for (int i = 1; i <= K; ++i) {
        if (group_counts[group_id][i] > 1) return false;
    }
    return true;
}

// Actually, since we update only one cell at a time, we can maintain group_valid directly
void update_marked_group_simple(int group_id, int num, int delta) {
    group_counts[group_id][num] += delta;
    // If adding, check if count becomes 2 -> invalid
    // If removing, check if count becomes 1 -> might be valid, but others might still be invalid
    // So simpler: recompute only if needed, or just check on conflict
    // Given small size (7 cells), we can recompute validity when needed
}

bool is_group_valid_fast(int group_id) {
    for (int i = 1; i <= K; ++i) {
        if (group_counts[group_id][i] > 1) return false;
    }
    return true;
}

// Backtracking function to count solutions up to N, and find the N-th
// Returns: number of solutions found (but we only need to find the N-th, so we can stop when we hit N)
// However, N can be up to 10^18, so we must not enumerate all if there are many
// But note: the board is fixed size (31 cells), and K is at most 31, but the number of solutions could be huge
// However, the constraints are very tight (each row/diag must have distinct numbers, plus marked groups)
// In practice, the number of solutions is not astronomical (sample input K=8, N=1 gives one solution)

// We'll do DFS in lexicographical order (fill cells 0 to 30, try numbers 1..K in order)
// We need to count solutions until we reach N

// To avoid TLE, we must prune early and use constraints effectively.

// Since the problem is small (31 cells), and with the constraints, backtracking should work if optimized.

// We'll use recursion with pruning, and stop when we found N solutions.

// Global state:
vector<int> solution; // current partial assignment
vector<int> board_copy;
int solutions_found = 0;
bool found_solution = false;
vector<int> nth_solution;

// Instead of global counters, we'll use DFS that returns count, but stops when count >= N

// We'll use iterative DFS? Or recursive with pruning.
// Recursive is simpler.

// Precompute constraint arrays
// used_in_row[i][num] = true if num used in row i
// Similarly for diag

vector<vector<bool>> used_row, used_slash, used_backslash;

bool dfs(int pos) {
    if (pos == NUM_CELLS) {
        solutions_found++;
        if (solutions_found == 1) {
            nth_solution = board;
        }
        return solutions_found >= N;
    }

    // Find next empty cell? Actually we go sequentially
    // So board[pos] might be fixed or empty

    int start_val = 1, end_val = K;
    if (board[pos] != 0) {
        // fixed cell
        if (!check_constraints(pos, board[pos])) {
            return false;
        }
        // update used structures
        for (int r : cell_to_rows[pos]) used_row[r][board[pos]] = true;
        for (int s : cell_to_slash[pos]) used_slash[s][board[pos]] = true;
        for (int b : cell_to_backslash[pos]) used_backslash[b][board[pos]] = true;
        
        // update marked groups
        for (int g : cell_to_marked_groups[pos]) {
            group_counts[g][board[pos]]++;
        }

        bool res = dfs(pos + 1);

        // revert
        for (int r : cell_to_rows[pos]) used_row[r][board[pos]] = false;
        for (int s : cell_to_slash[pos]) used_slash[s][board[pos]] = false;
        for (int b : cell_to_backslash[pos]) used_backslash[b][board[pos]] = false;

        for (int g : cell_to_marked_groups[pos]) {
            group_counts[g][board[pos]]--;
        }

        return res;
    }

    // Try numbers 1..K in order
    for (int val = 1; val <= K; ++val) {
        if (check_constraints(pos, val)) {
            board[pos] = val;

            // update used structures
            for (int r : cell_to_rows[pos]) used_row[r][val] = true;
            for (int s : cell_to_slash[pos]) used_slash[s][val] = true;
            for (int b : cell_to_backslash[pos]) used_backslash[b][val] = true;

            // update marked groups
            for (int g : cell_to_marked_groups[pos]) {
                group_counts[g][val]++;
            }

            // Check marked groups for this pos: only groups that include pos might have changed
            bool mark_ok = true;
            for (int g : cell_to_marked_groups[pos]) {
                // We can check if group_counts[g][val] > 1 -> invalid, but also check others?
                // Actually, only val might cause conflict, so if group_counts[g][val] > 1, invalid
                if (group_counts[g][val] > 1) {
                    mark_ok = false;
                    break;
                }
            }

            if (mark_ok) {
                if (dfs(pos + 1)) {
                    return true;
                }
            }

            // revert
            for (int r : cell_to_rows[pos]) used_row[r][val] = false;
            for (int s : cell_to_slash[pos]) used_slash[s][val] = false;
            for (int b : cell_to_backslash[pos]) used_backslash[b][val] = false;

            for (int g : cell_to_marked_groups[pos]) {
                group_counts[g][val]--;
            }

            board[pos] = 0;
        }
    }

    return false;
}

// Check if placing 'val' at pos is valid (before updating state)
bool check_constraints(int pos, int val) {
    // Check rows
    for (int r : cell_to_rows[pos]) {
        if (used_row[r][val]) return false;
    }
    // Check slash diagonals
    for (int s : cell_to_slash[pos]) {
        if (used_slash[s][val]) return false;
    }
    // Check backslash diagonals
    for (int b : cell_to_backslash[pos]) {
        if (used_backslash[b][val]) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    build_constraint_indices();

    while (cin >> K >> N) {
        board.resize(NUM_CELLS);
        for (int i = 0; i < NUM_CELLS; ++i) {
            cin >> board[i];
        }

        // Initialize used arrays
        int num_rows = rows.size();
        int num_slash = diag_slash.size();
        int num_backslash = diag_backslash.size();

        used_row = vector<vector<bool>>(num_rows, vector<bool>(K + 1, false));
        used_slash = vector<vector<bool>>(num_slash, vector<bool>(K + 1, false));
        used_backslash = vector<vector<bool>>(num_backslash, vector<bool>(K + 1, false));

        init_marked_groups();

        // Pre-check fixed cells for immediate conflict
        // Also, check if fixed cells satisfy the constraints before DFS
        bool pre_invalid = false;
        for (int i = 0; i < NUM_CELLS; ++i) {
            if (board[i] != 0) {
                // Check if board[i] is within [1, K]
                if (board[i] < 1 || board[i] > K) {
                    pre_invalid = true;
                }
                // Check constraints for fixed cells
                if (!check_constraints(i, board[i])) {
                    pre_invalid = true;
                    break;
                }
                // Update state for fixed cells
                for (int r : cell_to_rows[i]) used_row[r][board[i]] = true;
                for (int s : cell_to_slash[i]) used_slash[s][board[i]] = true;
                for (int b : cell_to_backslash[i]) used_backslash[b][board[i]] = true;
                for (int g : cell_to_marked_groups[i]) {
                    group_counts[g][board[i]]++;
                }
            }
        }

        if (pre_invalid) {
            cout << "No way" << endl;
            continue;
        }

        // Also, check marked groups for fixed cells
        bool mark_invalid = false;
        for (int g = 0; g < 7; ++g) {
            for (int num = 1; num <= K; ++num) {
                if (group_counts[g][num] > 1) {
                    mark_invalid = true;
                    break;
                }
            }
            if (mark_invalid) break;
        }

        if (mark_invalid) {
            cout << "No way" << endl;
            continue;
        }

        solutions_found = 0;
        found_solution = false;
        nth_solution.clear();

        // Do DFS starting from position 0
        if (dfs(0)) {
            cout << "Found" << endl;
            for (int i = 0; i < NUM_CELLS; ++i) {
                if (i > 0) cout << " ";
                cout << nth_solution[i];
            }
            cout << endl;
        } else {
            cout << "No way" << endl;
        }
    }

    return 0;
}
