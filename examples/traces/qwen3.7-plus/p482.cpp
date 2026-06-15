#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    
    vector<int> h(n);
    int total_sum = 0;
    for (int i = 0; i < n; ++i) {
        cin >> h[i];
        total_sum += h[i];
    }

    int P_initial = 2 * n + h[0] + h[n - 1];
    for (int i = 0; i < n - 1; ++i) {
        P_initial += abs(h[i] - h[i + 1]);
    }

    int P_target = P_initial / 2;

    vector<vector<int>> dp(n, vector<int>(P_target + 1, INF));
    vector<vector<int>> parent_i(n, vector<int>(P_target + 1, -1));
    vector<vector<int>> parent_P(n, vector<int>(P_target + 1, -1));

    for (int i = 0; i < n; ++i) {
        int P = min(P_target, 2 + h[i]);
        dp[i][P] = h[i];
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int P = 0; P <= P_target; ++P) {
                if (dp[i][P] != INF) {
                    int P_add = 2 + abs(h[i] - h[j]);
                    int P_new = min(P_target, P + P_add);
                    if (dp[i][P] + h[j] < dp[j][P_new]) {
                        dp[j][P_new] = dp[i][P] + h[j];
                        parent_i[j][P_new] = i;
                        parent_P[j][P_new] = P;
                    }
                }
            }
        }
    }

    int min_S = INF;
    int best_i = -1;
    int best_P = -1;

    for (int i = 0; i < n; ++i) {
        for (int P = 0; P <= P_target; ++P) {
            if (dp[i][P] != INF) {
                if (P + h[i] >= P_target) {
                    if (dp[i][P] < min_S) {
                        min_S = dp[i][P];
                        best_i = i;
                        best_P = P;
                    }
                }
            }
        }
    }

    if (min_S == INF) {
        min_S = total_sum;
        best_i = n - 1;
        best_P = P_target;
    }

    vector<int> kept_boards;
    int curr_i = best_i;
    int curr_P = best_P;
    while (curr_i != -1) {
        kept_boards.push_back(curr_i);
        int prev_i = parent_i[curr_i][curr_P];
        int prev_P = parent_P[curr_i][curr_P];
        curr_i = prev_i;
        curr_P = prev_P;
    }

    vector<bool> is_kept(n, false);
    for (int idx : kept_boards) {
        is_kept[idx] = true;
    }

    vector<int> stolen;
    int stolen_sum = 0;
    for (int i = 0; i < n; ++i) {
        if (!is_kept[i]) {
            stolen.push_back(i + 1);
            stolen_sum += h[i];
        }
    }

    cout << stolen_sum << "\n";
    cout << stolen.size() << "\n";
    for (size_t i = 0; i < stolen.size(); ++i) {
        cout << stolen[i] << (i + 1 == stolen.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
