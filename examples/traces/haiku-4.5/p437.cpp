#include<bits/stdc++.h>
using namespace std;

int K;
long long N;
vector<int> board(32);
vector<vector<int>> constraints;
vector<vector<int>> marked_groups;

void init_constraints() {
    // Horizontal rows
    constraints.push_back({1, 2});
    constraints.push_back({3, 4, 5, 6, 7});
    constraints.push_back({8, 9, 10, 11, 12, 13});
    constraints.push_back({14, 15, 16, 17, 18});
    constraints.push_back({19, 20, 21, 22, 23, 24});
    constraints.push_back({25, 26, 27, 28, 29});
    constraints.push_back({30, 31});
    
    // "/" diagonals
    constraints.push_back({3, 8});
    constraints.push_back({1, 4, 9, 14, 19});
    constraints.push_back({2, 5, 10, 15, 20, 25});
    constraints.push_back({6, 11, 16, 21, 26});
    constraints.push_back({7, 12, 17, 22, 27, 30});
    constraints.push_back({13, 18, 23, 28, 31});
    constraints.push_back({24, 29});
    
    // "\" diagonals
    constraints.push_back({7, 13});
    constraints.push_back({2, 6, 12, 18, 24});
    constraints.push_back({1, 5, 11, 17, 23, 29});
    constraints.push_back({4, 10, 16, 22, 28});
    constraints.push_back({3, 9, 15, 21, 27, 31});
    constraints.push_back({8, 14, 20, 26, 30});
    constraints.push_back({19, 25});
    
    // Marked groups
    marked_groups.push_back({1, 2, 4, 5, 6, 10, 11});
    marked_groups.push_back({3, 4, 8, 9, 10, 14, 15});
    marked_groups.push_back({6, 7, 11, 12, 13, 17, 18});
    marked_groups.push_back({10, 11, 15, 16, 17, 21, 22});
    marked_groups.push_back({14, 15, 19, 20, 21, 25, 26});
    marked_groups.push_back({17, 18, 22, 23, 24, 28, 29});
    marked_groups.push_back({21, 22, 26, 27, 28, 30, 31});
}

bool is_valid(int pos, int val) {
    board[pos] = val;
    
    // Check all constraints involving this position
    for (auto& constraint : constraints) {
        set<int> values;
        bool has_empty = false;
        for (int cell : constraint) {
            if (board[cell] == 0) {
                has_empty = true;
            } else {
                if (values.count(board[cell])) {
                    board[pos] = 0;
                    return false;
                }
                values.insert(board[cell]);
            }
        }
    }
    
    // Check marked groups
    for (auto& group : marked_groups) {
        set<int> values;
        bool has_empty = false;
        for (int cell : group) {
            if (board[cell] == 0) {
                has_empty = true;
            } else {
                if (values.count(board[cell])) {
                    board[pos] = 0;
                    return false;
                }
                values.insert(board[cell]);
            }
        }
    }
    
    board[pos] = 0;
    return true;
}

long long count_solutions(int pos, long long limit);

long long solve(int pos, long long& remaining) {
    if (pos == 32) {
        remaining--;
        return remaining == 0 ? 1 : 0;
    }
    
    if (board[pos] != 0) {
        return solve(pos + 1, remaining);
    }
    
    for (int val = 1; val <= K; val++) {
        if (is_valid(pos, val)) {
            board[pos] = val;
            if (solve(pos + 1, remaining) == 1) {
                return 1;
            }
            board[pos] = 0;
        }
    }
    
    return 0;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> K >> N;
    
    init_constraints();
    
    for (int i = 1; i <= 31; i++) {
        cin >> board[i];
    }
    
    long long remaining = N;
    if (solve(1, remaining) == 1) {
        cout << "Found\n";
        for (int i = 1; i <= 31; i++) {
            cout << board[i];
            if (i < 31) cout << " ";
        }
        cout << "\n";
    } else {
        cout << "No way\n";
    }
    
    return 0;
}
