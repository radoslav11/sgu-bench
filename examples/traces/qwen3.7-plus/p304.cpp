#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Tooth {
    int cost;
    int id;
    bool operator<(const Tooth& other) const {
        return cost < other.cost;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K, P;
    if (!(cin >> N >> K >> P)) return 0;

    vector<int> B(K + 1);
    for (int i = 1; i <= K; ++i) {
        cin >> B[i];
    }

    vector<vector<Tooth>> teeth(K + 1);
    for (int i = 1; i <= N; ++i) {
        int a, c;
        cin >> a >> c;
        teeth[c].push_back({a, i});
    }

    for (int i = 1; i <= K; ++i) {
        sort(teeth[i].begin(), teeth[i].end());
    }

    vector<vector<int>> cost(K + 1, vector<int>(N + 2, 0));
    for (int i = 1; i <= K; ++i) {
        int sum = 0;
        for (size_t k = 1; k <= teeth[i].size(); ++k) {
            sum += teeth[i][k - 1].cost;
            cost[i][k] = B[i] + sum;
        }
    }

    vector<vector<int>> dp(K + 1, vector<int>(N + 1, INF));
    vector<vector<int>> choice(K + 1, vector<int>(N + 1, 0));

    dp[0][0] = 0;
    int current_total = 0;

    for (int i = 1; i <= K; ++i) {
        int cnt = teeth[i].size();
        for (int c = 0; c <= current_total; ++c) {
            if (dp[i - 1][c] == INF) continue;
            
            if (dp[i - 1][c] < dp[i][c]) {
                dp[i][c] = dp[i - 1][c];
                choice[i][c] = 0;
            }

            for (int k = 1; k <= cnt; ++k) {
                if (c + k <= N) {
                    if (dp[i - 1][c] + cost[i][k] < dp[i][c + k]) {
                        dp[i][c + k] = dp[i - 1][c] + cost[i][k];
                        choice[i][c + k] = k;
                    }
                }
            }
        }
        current_total += cnt;
    }

    int max_teeth = 0;
    for (int c = N; c >= 0; --c) {
        if (dp[K][c] <= P) {
            max_teeth = c;
            break;
        }
    }

    cout << max_teeth << "\n";
    
    vector<int> result;
    int c = max_teeth;
    for (int i = K; i >= 1; --i) {
        int k = choice[i][c];
        if (k > 0) {
            for (int j = 0; j < k; ++j) {
                result.push_back(teeth[i][j].id);
            }
        }
        c -= k;
    }
    
    sort(result.begin(), result.end());
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 == result.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
