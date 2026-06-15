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
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

const int INF = 1e9;

int m, n;
vector<vector<char>> cake;

void read() {
    cin >> m >> n;
    cin.ignore();
    cake = vector<vector<char>>(m, vector<char>(n));
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            cin >> cake[i][j];
        }
        cin.ignore();
    }
}

bool check_failure(int i, int j, int mask, int window_size) {
    if(i * n + j >= window_size) {
        if(!(mask & (1 << window_size)) && !(mask & (1 << (window_size - 1))) &&
           j != n - 1) {
            return true;
        }
        if(!(mask & (1 << window_size)) && !(mask & (1 << (window_size - n)))) {
            return true;
        }
    }

    return false;
}

void solve() {
    /*
     * Broken-profile DP over cells in row-major order. The DP mask holds the
     * occupancy of the last 2*n cells (a sliding window of two rows): bit 0 is
     * the cell just before the current one, going back up to the cell n rows
     * above. curr_dp[mask] = minimum chocolate pieces used to reach the current
     * cell with that window occupancy.
     *
     * At each cell we shift the window left by one and decide its state:
     *   - a candle ('*') is forced occupied;
     *   - an empty cell can be left empty, covered vertically by a piece with
     *     the cell directly above, or horizontally with the cell to its left.
     * check_failure rejects transitions that leave a cell finalized while it
     * and an already-fixed neighbour are both empty (an addable piece would
     * remain). After the sweep we keep masks whose trailing two rows admit no
     * further placeable piece and take the minimum count.
     */

    int window_size = 2 * n, full_mask = (1 << window_size) - 1;
    vector<int> curr_dp(1 << window_size, INF);
    vector<int> next_dp(1 << window_size, INF);

    curr_dp[0] = 0;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            next_dp.assign(1 << window_size, INF);
            for(int mask = 0; mask < (1 << window_size); mask++) {
                if(curr_dp[mask] == INF) {
                    continue;
                }

                if(cake[i][j] == '*') {
                    int new_mask = mask << 1 | 1;
                    if(!check_failure(i, j, new_mask, window_size)) {
                        new_mask &= full_mask;
                        next_dp[new_mask] =
                            min(next_dp[new_mask], curr_dp[mask]);
                    }
                } else {
                    {
                        int new_mask = mask << 1;
                        if(!check_failure(i, j, new_mask, window_size)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask]);
                        }
                    }

                    if(i > 0 && !(mask & (1 << (n - 1)))) {
                        int new_mask = (mask | (1 << (n - 1))) << 1 | 1;
                        if(!check_failure(i, j, new_mask, window_size)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask] + 1);
                        }
                    }

                    if(j > 0 && !(mask & 1)) {
                        int new_mask = mask << 1 | 3;
                        if(!check_failure(i, j, new_mask, window_size)) {
                            new_mask &= full_mask;
                            next_dp[new_mask] =
                                min(next_dp[new_mask], curr_dp[mask] + 1);
                        }
                    }
                }
            }

            curr_dp = std::move(next_dp);
        }
    }

    int result = INF;
    for(int mask = 0; mask < (1 << window_size); mask++) {
        if(curr_dp[mask] == INF) {
            continue;
        }

        bool possible_tile = false;
        for(int i = 0; i < n; i++) {
            if(m != 1 && !(mask & (1 << i)) && !(mask & (1 << (i + n)))) {
                possible_tile = true;
                break;
            }

            if(i != n - 1 && !(mask & (1 << i)) && !(mask & (1 << (i + 1)))) {
                possible_tile = true;
                break;
            }

            if(i != n - 1 && m != 1 && !(mask & (1 << (i + n))) &&
               !(mask & (1 << (i + n + 1)))) {
                possible_tile = true;
                break;
            }
        }

        if(!possible_tile) {
            result = min(result, curr_dp[mask]);
        }
    }

    assert(result != INF);
    cout << result << endl;
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
