#include <iostream>
#include <vector>
using namespace std;

int main() {
    int S;
    cin >> S;

    // dp[j]: count of sequences (in non-decreasing phase) of current length ending with digit j
    // dp2[j]: count of sequences (in non-increasing phase) of current length ending with digit j
    vector<long long> dp(10, 1);
    vector<long long> dp2(10, 0);

    // Build sequences of length 2 to S
    for (int i = 2; i <= S; i++) {
        vector<long long> new_dp(10, 0);
        vector<long long> new_dp2(10, 0);

        // Compute prefix sums for dp: new_dp[j] = sum_{k=0}^{j} dp[k]
        long long prefix = 0;
        for (int j = 0; j < 10; j++) {
            prefix += dp[j];
            new_dp[j] = prefix;
        }

        // Compute suffix sums for dp and dp2: from high digit to low
        long long suffix_dp = 0;
        long long suffix_dp2 = 0;
        for (int j = 9; j >= 0; j--) {
            suffix_dp += dp[j];
            suffix_dp2 += dp2[j];
            // From non-decreasing phase: we can only transition by decreasing (so next digit < last digit)
            //   so we take digits from j+1 to 9 from dp (which is suffix_dp - dp[j])
            // From non-increasing phase: we can take any digit <= j from dp2 (which is suffix_dp2)
            new_dp2[j] = (suffix_dp - dp[j]) + suffix_dp2;
        }

        dp = new_dp;
        dp2 = new_dp2;
    }

    long long ans = 0;
    for (int j = 0; j < 10; j++) {
        ans += dp[j] + dp2[j];
    }
    cout << ans << endl;

    return 0;
}
