#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

/**
 * Problem: 343. VaR (Value at Risk)
 * 
 * The objective is to calculate the Value at Risk (VaR) for a given portfolio of assets.
 * The VaR formula provided is:
 * VaR = -VP * (mP - 1.644854 * sP)
 * 
 * Parameters:
 * - VP: Initial value of the portfolio (sum of initial values of each asset).
 * - mP: Expected return of the portfolio, calculated as a weighted sum of the expected 
 *   returns of individual assets.
 * - sP: Standard deviation of the portfolio's return.
 * - w_i: Weight of asset i in the portfolio (V_i / VP).
 * - r_i(t): Return of asset i on day t, defined as (p_{t-1}(i) - p_t(i)) / p_t(i).
 * 
 * Inputs:
 * - T: Number of historical days.
 * - N: Number of assets.
 * - quantities of each asset in the portfolio.
 * - T+1 lines of prices (day 0 is today, day 1 is yesterday, ..., day T is T days ago).
 * 
 * Complexity:
 * - Time: O(T * N), where T <= 10^4 and N <= 10. This is well within the 0.25s time limit.
 * - Space: O(T * N), about 1-2 MB. This fits within 64 MB.
 */

using namespace std;

int main() {
    // Speed up I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T, N;
    if (!(cin >> T >> N)) return 0;

    // Quantity of each asset in the portfolio
    vector<long double> q(N);
    for (int i = 0; i < N; ++i) {
        cin >> q[i];
    }

    // Prices: p[t][i] is the price of asset i on day t
    // T+1 lines of prices (day 0 to day T)
    vector<vector<long double>> p(T + 1, vector<long double>(N));
    for (int t = 0; t <= T; ++t) {
        for (int i = 0; i < N; ++i) {
            cin >> p[t][i];
        }
    }

    // Initial values of each asset and the total portfolio value (using today's prices, day 0)
    vector<long double> V(N);
    long double VP = 0;
    for (int i = 0; i < N; ++i) {
        V[i] = q[i] * p[0][i];
        VP += V[i];
    }

    // Weights of each asset in the portfolio
    vector<long double> w(N);
    for (int i = 0; i < N; ++i) {
        w[i] = V[i] / VP;
    }

    // Calculate the historical returns for each asset for each day t = 1 to T
    // r[t-1][i] corresponds to the return on day t.
    // r_i(t) = (p_{t-1}(i) - p_t(i)) / p_t(i)
    vector<vector<long double>> r(T, vector<long double>(N));
    for (int t = 1; t <= T; ++t) {
        for (int i = 0; i < N; ++i) {
            r[t - 1][i] = (p[t - 1][i] - p[t][i]) / p[t][i];
        }
    }

    // Calculate the portfolio return for each day: R_P(t) = sum_i (w_i * r_i(t))
    vector<long double> RP(T, 0.0L);
    for (int t = 0; t < T; ++t) {
        for (int i = 0; i < N; ++i) {
            RP[t] += w[i] * r[t][i];
        }
    }

    // Calculate the expected return of the portfolio mP = mean(RP)
    long double mP = 0;
    for (int t = 0; t < T; ++t) {
        mP += RP[t];
    }
    mP /= (long double)T;

    // Calculate the standard deviation of the portfolio's return sP = sqrt(Var(RP))
    long double varianceP = 0;
    for (int t = 0; t < T; ++t) {
        long double diff = RP[t] - mP;
        varianceP += diff * diff;
    }
    varianceP /= (long double)T;
    long double sP = sqrt(varianceP);

    // Calculate Value at Risk using the formula provided in the problem
    // VaR = -VP * (mP - 1.644854 * sP)
    long double VaR = -VP * (mP - 1.644854L * sP);

    // Output the result with at least 2 decimal places
    cout << fixed << setprecision(2) << (double)VaR << endl;

    return 0;
}

