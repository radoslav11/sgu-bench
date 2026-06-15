#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int simulate(int r_ill, int N, int W, int R, int C, const vector<int>& L) {
    int curr_row = 1;
    int curr_col = 1;
    int prev_row = 0;
    int prev_end = 0;
    int max_row = r_ill + R - 1;

    for (int i = 0; i < N; ++i) {
        int len = L[i];
        if (prev_row > 0) {
            curr_row = prev_row;
            int start_c = prev_end + 2;
            int limit = (curr_row >= r_ill && curr_row < r_ill + R) ? (W - C) : W;
            
            if (start_c + len - 1 <= limit) {
                curr_col = start_c;
            } else {
                curr_row = prev_row + 1;
                if (curr_row >= r_ill && curr_row < r_ill + R) {
                    if (len <= W - C) {
                        curr_col = 1;
                    } else {
                        curr_row = r_ill + R;
                        curr_col = 1;
                    }
                } else {
                    curr_col = 1;
                }
            }
        } else {
            curr_row = 1;
            if (curr_row >= r_ill && curr_row < r_ill + R) {
                if (len <= W - C) {
                    curr_col = 1;
                } else {
                    curr_row = r_ill + R;
                    curr_col = 1;
                }
            } else {
                curr_col = 1;
            }
        }
        prev_row = curr_row;
        prev_end = curr_col + len - 1;
        if (curr_row > max_row) {
            max_row = curr_row;
        }
    }
    return max_row;
}

void solve() {
    int N, W, R, C;
    if (!(cin >> N >> W >> R >> C)) return;
    vector<int> L(N);
    for (int i = 0; i < N; ++i) {
        cin >> L[i];
    }

    vector<int> cand;
    cand.push_back(1);
    int curr_row = 1;
    int curr_col = 1;
    int prev_row = 0;
    int prev_end = 0;
    
    for (int i = 0; i < N; ++i) {
        int len = L[i];
        bool placed = false;
        if (prev_row > 0) {
            int start_c = prev_end + 2;
            if (start_c + len - 1 <= W) {
                curr_row = prev_row;
                curr_col = start_c;
                placed = true;
            }
        }
        if (!placed) {
            curr_row = (prev_row > 0) ? prev_row + 1 : 1;
            curr_col = 1;
            placed = true;
        }
        cand.push_back(curr_row);
        prev_row = curr_row;
        prev_end = curr_col + len - 1;
    }
    
    sort(cand.begin(), cand.end());
    cand.erase(unique(cand.begin(), cand.end()), cand.end());
    
    int min_rows = 1e9;
    for (int r : cand) {
        min_rows = min(min_rows, simulate(r, N, W, R, C, L));
    }
    
    cout << min_rows << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    if (cin >> T) {
        while (T--) {
            solve();
        }
    }
    return 0;
}
