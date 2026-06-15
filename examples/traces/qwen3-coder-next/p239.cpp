#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }

    // dp[prev2][prev1]: number of ways for the state where the last two mines (x_{i-2}=prev2, x_{i-1}=prev1)
    // For the first cell (i=0), we use a virtual x_{-1}=0, so state is (0, x0)
    long long dp[2][2] = {{0}};
    dp[0][0] = 1;
    dp[0][1] = 1;

    // Process from the second cell (i=1) to the last cell (i=N-1)
    for (int i = 1; i < N; i++) {
        long long new_dp[2][2] = {{0}};
        for (int prev2 = 0; prev2 < 2; prev2++) {
            for (int prev1 = 0; prev1 < 2; prev1++) {
                for (int curr = 0; curr < 2; curr++) {
                    // Check constraint for row (i-1): a[i-1] = prev2 + prev1 + curr
                    if (prev2 + prev1 + curr == a[i-1]) {
                        new_dp[prev1][curr] += dp[prev2][prev1];
                    }
                }
            }
        }
        // Update dp to new_dp
        for (int i1 = 0; i1 < 2; i1++) {
            for (int i2 = 0; i2 < 2; i2++) {
                dp[i1][i2] = new_dp[i1][i2];
            }
        }
    }

    long long ans = 0;
    for (int prev1 = 0; prev1 < 2; prev1++) {
        for (int curr = 0; curr < 2; curr++) {
            // Check constraint for the last row (N-1): a[N-1] = prev1 + curr
            if (prev1 + curr == a[N-1]) {
                ans += dp[prev1][curr];
            }
        }
    }

    cout << ans << endl;
    return 0;
}
