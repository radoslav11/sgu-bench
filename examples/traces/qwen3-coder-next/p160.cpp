#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> a(N);
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }

    // dp[i][r]: whether remainder r is achievable using first i levers
    vector<vector<char>> dp(N+1, vector<char>(M, 0));
    // prev_rem[i][r]: the remainder before adding lever i that led to r (if lever i was taken)
    vector<vector<int>> prev_rem(N+1, vector<int>(M, -1));

    // Base case: 0 levers, score 1
    dp[0][1] = 1;

    for (int i = 1; i <= N; i++) {
        int ai = a[i-1];
        // Copy previous state (not taking lever i)
        for (int r = 0; r < M; r++) {
            dp[i][r] = dp[i-1][r];
        }
        // Try taking lever i
        for (int r = 0; r < M; r++) {
            if (dp[i-1][r]) {
                int nr = (r * ai) % M;
                if (!dp[i][nr]) {
                    dp[i][nr] = 1;
                    prev_rem[i][nr] = r;
                }
            }
        }
    }

    // Find maximum achievable remainder
    int max_rem = 0;
    for (int r = M-1; r >= 0; r--) {
        if (dp[N][r]) {
            max_rem = r;
            break;
        }
    }

    // Backtrack to find levers
    vector<int> levers;
    int r = max_rem;
    for (int i = N; i >= 1; i--) {
        if (!dp[i-1][r]) { // lever i was taken
            levers.push_back(i);
            r = prev_rem[i][r];
        }
    }

    // Output
    cout << max_rem << endl;
    // Output levers in ascending order (backtrack gives descending, so reverse)
    if (!levers.empty()) {
        sort(levers.begin(), levers.end());
        for (int i = 0; i < levers.size(); i++) {
            if (i > 0) cout << " ";
            cout << levers[i];
        }
        cout << endl;
    } else {
        cout << endl;
    }

    return 0;
}
