#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto& x: a) {
        out << x << ' ';
    }
    return out;
}

int n, k;
vector<pair<int, int>> black_cells, white_cells;
vector<bool> used_diag1_black, used_diag2_black;
vector<bool> used_diag1_white, used_diag2_white;
vector<int64_t> ways_black, ways_white;

void read() { cin >> n >> k; }

void backtrack_black(int idx, int placed) {
    if(idx == (int)black_cells.size()) {
        if(placed <= k) {
            ways_black[placed]++;
        }

        return;
    }

    backtrack_black(idx + 1, placed);

    auto [r, c] = black_cells[idx];
    int d1 = r + c;
    int d2 = r - c + (n - 1);
    if(!used_diag1_black[d1] && !used_diag2_black[d2]) {
        used_diag1_black[d1] = used_diag2_black[d2] = true;
        backtrack_black(idx + 1, placed + 1);
        used_diag1_black[d1] = used_diag2_black[d2] = false;
    }
}

void backtrack_white(int idx, int placed) {
    if(idx == (int)white_cells.size()) {
        if(placed <= k) {
            ways_white[placed]++;
        }

        return;
    }

    backtrack_white(idx + 1, placed);

    auto [r, c] = white_cells[idx];
    int d1 = r + c;
    int d2 = r - c + (n - 1);
    if(!used_diag1_white[d1] && !used_diag2_white[d2]) {
        used_diag1_white[d1] = used_diag2_white[d2] = true;
        backtrack_white(idx + 1, placed + 1);
        used_diag1_white[d1] = used_diag2_white[d2] = false;
    }
}

void solve() {
    // Bishops attack along diagonals. Rotating the board 45 degrees, the two
    // diagonal directions become rows and columns, and the board splits into
    // two independent sub-problems: cells of one colour (by (r+c) parity) never
    // share a diagonal with cells of the other colour. So we enumerate, for
    // each colour separately, how many ways there are to place exactly i
    // non-attacking bishops on that colour's cells, recording the counts in
    // ways_black[i] / ways_white[i] via backtracking that tracks which "/" and
    // "\" diagonals are already occupied. The total number of placements of k
    // bishops is then the convolution sum over i of
    // ways_black[i] * ways_white[k - i].

    black_cells.clear();
    white_cells.clear();
    for(int r = 0; r < n; r++) {
        for(int c = 0; c < n; c++) {
            if(((r + c) & 1) == 0) {
                black_cells.emplace_back(r, c);
            } else {
                white_cells.emplace_back(r, c);
            }
        }
    }

    used_diag1_black.assign(2 * n, false);
    used_diag2_black.assign(2 * n, false);
    used_diag1_white.assign(2 * n, false);
    used_diag2_white.assign(2 * n, false);

    ways_black.assign(k + 1, 0);
    ways_white.assign(k + 1, 0);

    backtrack_black(0, 0);
    backtrack_white(0, 0);

    int64_t ans = 0;
    for(int i = 0; i <= k; i++) {
        ans += ways_black[i] * ways_white[k - i];
    }

    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
