#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    string A_str;
    long long B_val;
    cin >> A_str;
    cin >> B_val;

    // Special case: B = 1
    if (B_val == 1) {
        cout << A_str << endl;
        return 0;
    }

    // Convert A_str to base B_val, LSB first
    vector<int> digits;
    string num = A_str;
    while (num != "0") {
        int rem = 0;
        for (int i = 0; i < num.size(); i++) {
            rem = rem * 10 + (num[i] - '0');
            num[i] = (rem / B_val) + '0';
            rem %= B_val;
        }
        digits.push_back(rem);
        // Remove leading zeros
        int start = 0;
        while (start < num.size() && num[start] == '0') 
            start++;
        if (start == (int)num.size()) 
            num = "0";
        else 
            num = num.substr(start);
    }

    // If the number is zero, but the problem states positive integers, so digits should not be empty.
    // DP setup
    const int MAX_CARRY = 6;
    int n = digits.size();
    // dp[i][c] for i in [0, n], c in [-MAX_CARRY, MAX_CARRY] -> index = c + MAX_CARRY
    vector<vector<long long>> dp(n+1, vector<long long>(2*MAX_CARRY+1, LLONG_MAX));
    dp[0][0 + MAX_CARRY] = 0;

    for (int i = 0; i < n; i++) {
        for (int c = -MAX_CARRY; c <= MAX_CARRY; c++) {
            if (dp[i][c + MAX_CARRY] == LLONG_MAX) 
                continue;
            int d = digits[i] + c;
            // Compute r1 in [0, B_val-1]
            int r1 = d % (int)B_val;
            if (r1 < 0) 
                r1 += (int)B_val;
            int c_out1 = (d - r1) / (int)B_val;
            int r2 = r1 - (int)B_val;
            int c_out2 = c_out1 + 1;

            // Option 1: use r1
            if (c_out1 >= -MAX_CARRY && c_out1 <= MAX_CARRY) {
                long long new_cost = dp[i][c + MAX_CARRY] + abs(r1);
                if (new_cost < dp[i+1][c_out1 + MAX_CARRY]) {
                    dp[i+1][c_out1 + MAX_CARRY] = new_cost;
                }
            }

            // Option 2: use r2
            if (c_out2 >= -MAX_CARRY && c_out2 <= MAX_CARRY) {
                long long new_cost = dp[i][c + MAX_CARRY] + abs(r2);
                if (new_cost < dp[i+1][c_out2 + MAX_CARRY]) {
                    dp[i+1][c_out2 + MAX_CARRY] = new_cost;
                }
            }
        }
    }

    long long ans = LLONG_MAX;
    for (int c = -MAX_CARRY; c <= MAX_CARRY; c++) {
        if (dp[n][c + MAX_CARRY] == LLONG_MAX) 
            continue;
        long long total = dp[n][c + MAX_CARRY] + abs(c);
        if (total < ans) 
            ans = total;
    }
    cout << ans << endl;

    return 0;
}
