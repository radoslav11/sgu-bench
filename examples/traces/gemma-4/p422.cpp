#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * Vasya is typing a text of n characters. For each character i, there's a probability a[i] of a typo.
 * He can decide to type some number of characters and then look at the screen.
 * Looking at the screen takes t time.
 * If he finds the first typo at position k, he backspaces from the current position j down to k,
 * which takes j-k+1 backspace presses, and starts typing from position k again.
 * The goal is to minimize the expected time to type the entire text correctly.
 * 
 * Let dp[i] be the minimal expected time to type characters from i to n correctly,
 * given that characters 1 to i-1 are already correctly typed.
 * dp[n+1] = 0.
 * 
 * For each i, we decide to type characters up to position j (i <= j <= n) and then look at the screen.
 * The cost of this strategy is:
 * Cost(i, j) = (j-i+1) + t + Sum_{k=i}^j P(i, k-1) * a[k] * (j-k+1 + dp[k]) + P(i, j) * dp[j+1]
 * where P(i, j) = Product_{m=i}^j (1-a[m]).
 * 
 * Let S(i, j) = Sum_{k=i}^j P(i, k-1) * a[k] * (j-k+1 + dp[k]).
 * We use the identity Sum_{k=i}^j P(i, k-1) * a[k] * (j-k+1) = (j-i+1) - Sum_{k=i}^j P(i, k).
 * 
 * Thus, dp[i] = min_{j=i}^n { (j-i+1) + t + (j-i+1) - Sum_{k=i}^j P(i, k) + Sum_{k=i}^j P(i, k-1) * a[k] * dp[k] + P(i, j) * dp[j+1] }
 * dp[i] = min_{j=i}^n { 2*(j-i+1) + t - Sum_{k=i}^j P(i, k) + Sum_{k=i}^j P(i, k-1) * a[k] * dp[k] + P(i, j) * dp[j+1] }
 * 
 * For the k=i term in the summation: P(i, i-1)*a[i]*dp[i] = a[i]*dp[i].
 * To solve for dp[i], isolate dp[i] from the equation:
 * dp[i] * (1 - a[i]) = min_{j=i}^n { 2*(j-i+1) + t - Sum_{k=i}^j P(i, k) + Sum_{k=i+1}^j P(i, k-1) * a[k] * dp[k] + P(i, j) * dp[j+1] }
 * (Actually the formula is slightly different for the j=i case when isolating dp[i], but the logic remains the same).
 * Let's re-check the j=i case:
 * dp[i] = (1) + t + a[i]*(1 + dp[i]) + (1-a[i])*dp[i+1]
 * dp[i]*(1-a[i]) = 1 + t + a[i] + (1-a[i])*dp[i+1].
 * This matches the simplified dp[i] formula.
 * 
 * The complexity is O(n^2), which should fit in the 0.25s time limit with tight loops.
 */

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    double t;
    if (!(cin >> n >> t)) return 0;

    vector<double> a(n + 1);
    vector<double> a_not(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        a_not[i] = 1.0 - a[i];
    }

    vector<double> dp(n + 2, 0.0);

    for (int i = n; i >= 1; --i) {
        double a_i = a[i];
        double P_i_i = a_not[i];
        
        // The case where the first typo is at position i:
        // The formula for dp[i](1-a[i]) for j=i:
        // 2*(1) + t - P(i,i) + 0 + P(i,i)*dp[i+1]
        // = 2 + t - (1-a[i]) + (1-a[i])*dp[i+1] = 1 + t + a[i] + (1-a[i])*dp[i+1]
        
        double current_sum_P = P_i_i;
        double current_sum_a_dp = 0.0;
        double P_prev = P_i_i;
        double min_val = 2.0 + t - P_i_i + P_i_i * dp[i + 1];

        // For j = i + 1 to n:
        for (int j = i + 1; j <= n; ++j) {
            double next_P = P_prev * a_not[j];
            current_sum_P += next_P;
            current_sum_a_dp += (P_prev - next_P) * dp[j];
            
            double val = 2.0 * (j - i + 1) + t - current_sum_P + current_sum_a_dp + next_P * dp[j + 1];
            if (val < min_val) min_val = val;
            P_prev = next_P;
        }
        
        // If a[i] is 1, dp[i] is infinite.
        if (a_i > 1.0 - 1e-18) {
            dp[i] = 1e18; 
        } else {
            dp[i] = min_val / (1.0 - a_i);
        }
    }

    cout << fixed << setprecision(12) << dp[1] << endl;

    return 0;
}

