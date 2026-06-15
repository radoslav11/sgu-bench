#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> T(n, vector<int>(m));
    vector<int> all_vals;
    all_vals.reserve(n * m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> T[i][j];
            all_vals.push_back(T[i][j]);
        }
    }

    if (n == 1) {
        cout << "Yes\n";
        sort(all_vals.begin(), all_vals.end());
        all_vals.erase(unique(all_vals.begin(), all_vals.end()), all_vals.end());
        for (int x : all_vals) {
            cout << x << " -> 0\n";
        }
        return 0;
    }

    sort(all_vals.begin(), all_vals.end());
    all_vals.erase(unique(all_vals.begin(), all_vals.end()), all_vals.end());
    
    int num_vals = all_vals.size();
    vector<int> f(num_vals, 0);
    vector<vector<pair<int, int>>> occ(num_vals);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int val = lower_bound(all_vals.begin(), all_vals.end(), T[i][j]) - all_vals.begin();
            T[i][j] = val;
            occ[val].push_back({i, j});
        }
    }

    mt19937 rng(1337);

    auto solve = [&]() {
        for (int i = 0; i < num_vals; ++i) {
            f[i] = rng() % 51;
        }

        vector<int> diff_count(n, 0);
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (T[0][j] != T[i][j] && f[T[0][j]] != f[T[i][j]]) {
                    diff_count[i]++;
                }
            }
        }

        vector<int> bad_rows;
        for (int i = 1; i < n; ++i) {
            if (diff_count[i] == 0) {
                bad_rows.push_back(i);
            }
        }
        
        for (int step = 0; step < 10000; ++step) {
            vector<int> next_bad;
            next_bad.reserve(bad_rows.size());
            for (int i : bad_rows) {
                if (diff_count[i] == 0) {
                    next_bad.push_back(i);
                }
            }
            bad_rows = move(next_bad);
            
            if (bad_rows.empty()) {
                return true;
            }
            
            int bad_i = bad_rows[rng() % bad_rows.size()];
            
            int best_j = -1;
            int min_occ = 1e9;
            for (int k = 0; k < m; ++k) {
                if (T[0][k] != T[bad_i][k]) {
                    int cand_x = T[bad_i][k];
                    if ((int)occ[cand_x].size() < min_occ) {
                        min_occ = occ[cand_x].size();
                        best_j = k;
                    }
                }
            }
            
            int x = T[bad_i][best_j];
            int y = T[0][best_j];

            int old_color = f[x];
            int new_color = rng() % 50;
            if (new_color >= f[y]) new_color++;

            if (old_color == new_color) continue;

            for (auto& p : occ[x]) {
                int r = p.first;
                if (r == 0) continue;
                int c = p.second;
                int z = T[0][c];
                if (z == x) continue;

                bool was_diff = (f[z] != old_color);
                bool now_diff = (f[z] != new_color);

                if (was_diff && !now_diff) {
                    diff_count[r]--;
                } else if (!was_diff && now_diff) {
                    diff_count[r]++;
                }
            }

            f[x] = new_color;
        }
        return false;
    };

    bool found = false;
    for (int iter = 0; iter < 100; ++iter) {
        if (solve()) {
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Yes\n";
        for (int i = 0; i < num_vals; ++i) {
            cout << all_vals[i] << " -> " << f[i] << "\n";
        }
    } else {
        cout << "No\n";
    }

    return 0;
}
