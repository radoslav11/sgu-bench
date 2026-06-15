#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * Two businessmen want to maximize their expected profit, defined as:
 * E[Profit] = V * P(win) - E[total bribes]
 * 
 * The game can be modeled as a dynamic programming problem over states (n1, n2), 
 * where n1 and n2 are the number of approvals each businessman has already obtained.
 * 
 * The state space is (N+1) x (N+1). We solve from (N, N) backwards to (0, 0).
 * For each state (n1, n2) where n1 < N and n2 < N, we find the Nash equilibrium (p1, p2) 
 * which maximizes businessman 1's and businessman 2's respective expected profits.
 * 
 * The bribe b can be any non-negative real number.
 * The probability of approval on a day with bribe b is p = 1 - 0.99 * (1 - F)^b.
 * This implies b = log((1 - p) / 0.99) / log(1 - F).
 * 
 * The expected profit E1(n1, n2) satisfies:
 * E1 = [p1 * p2 * V1++ + p1 * (1-p2) * V1+ + (1-p1) * p2 * V1- - b1] / (p1 + p2 - p1 * p2)
 * where V1++, V1+, V1- are the expected future profits for businessman 1 
 * after the outcomes (both get, only 1 gets, only 2 gets, neither gets).
 * 
 * The tie-breaking rule for N-th approval on the same day results in V/2.
 * 
 * Due to the small state space (N <= 10), we solve for (p1, p2) using a Gauss-Seidel-like iterative approach.
 */

double N_val, F1, F2, V_val;
int N;
double E1[12][12], E2[12][12], W1[12][12];

double get_p(double b, double F) {
    return 1.0 - 0.99 * pow(1.0 - F, b);
}

double get_b(double p, double F) {
    if (p >= 1.0 - 1e-12) return 1000.0;
    if (p <= 0.01) return 0.0;
    return log((1.0 - p) / 0.99) / log(1.0 - F);
}

double calc_E1(double b1, double p2, int n1, int n2) {
    double p1 = get_p(b1, F1);
    double V1_pp, V1_p, V1_m;
    
    if (n1 + 1 == N && n2 + 1 == N) V1_pp = V_val / 2.0;
    else if (n1 + 1 == N) V1_pp = V_val;
    else if (n2 + 1 == N) V1_pp = 0.0;
    else V1_pp = E1[n1 + 1][n2 + 1];
    
    if (n1 + 1 == N) V1_p = V_val;
    else V1_p = E1[n1 + 1][n2];
    
    if (n2 + 1 == N) V1_m = 0.0;
    else V1_m = E1[n1][n2 + 1];
    
    double num = p1 * p2 * V1_pp + p1 * (1.0 - p2) * V1_p + (1.0 - p1) * p2 * V1_m - b1;
    double den = p1 + p2 - p1 * p2;
    return num / den;
}

double calc_E2(double b2, double p1, int n1, int n2) {
    double p2 = get_p(b2, F2);
    double V2_pp, V2_p, V2_m;
    
    if (n1 + 1 == N && n2 + 1 == N) V2_pp = V_val / 2.0;
    else if (n2 + 1 == N) V2_pp = V_val;
    else if (n1 + 1 == N) V2_pp = 0.0;
    else V2_pp = E2[n1 + 1][n2 + 1];
    
    if (n2 + 1 == N) V2_p = V_val;
    else V2_p = E2[n1][n2 + 1];
    
    if (n1 + 1 == N) V2_m = 0.0;
    else V2_m = E2[n1 + 1][n2];
    
    double num = p1 * p2 * V2_pp + (1.0 - p1) * p2 * V2_m + p1 * (1.0 - p2) * V2_p - b2;
    double den = p1 + p2 - p1 * p2;
    return num / den;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> F1 >> F2 >> V_val)) return 0;

    // Base cases for DP
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= N; ++j) {
            if (i == N && j == N) {
                E1[i][j] = V_val / 2.0;
                E2[i][j] = V_val / 2.0;
                W1[i][j] = 0.5;
            } else if (i == N) {
                E1[i][j] = V_val;
                E2[i][j] = 0.0;
                W1[i][j] = 1.0;
            } else if (j == N) {
                E1[i][j] = 0.0;
                E2[i][j] = V_val;
                W1[i][j] = 0.0;
            }
        }
    }

    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j >= 0; --j) {
            double p1 = 0.5, p2 = 0.5;
            // Iteratively find Nash equilibrium
            for (int iter = 0; iter < 80; ++iter) {
                // Maximize E1(b1, p2) using ternary search
                double low = 0, high = 1000;
                for (int ts = 0; ts < 50; ++ts) {
                    double m1 = low + (high - low) / 3.0;
                    double m2 = high - (high - low) / 3.0;
                    if (calc_E1(m1, p2, i, j) < calc_E1(m2, p2, i, j)) low = m1;
                    else high = m2;
                }
                p1 = get_p(low, F1);

                // Maximize E2(p1, b2) using ternary search
                low = 0, high = 1000;
                for (int ts = 0; ts < 50; ++ts) {
                    double m1 = low + (high - low) / 3.0;
                    double m2 = high - (high - low) / 3.0;
                    if (calc_E2(m1, p1, i, j) < calc_E2(m2, p1, i, j)) low = m1;
                    else high = m2;
                }
                p2 = get_p(low, F2);
            }

            // Calculate final values for the state
            double b1_opt = get_b(p1, F1);
            double b2_opt = get_b(p2, F2);
            
            // Denominator is the probability of at least one getting an approval
            double den = p1 + p2 - p1 * p2;
            
            double V1_pp, V1_p, V1_m;
            if (i + 1 == N && j + 1 == N) V1_pp = V_val / 2.0;
            else if (i + 1 == N) V1_pp = V_val;
            else if (j + 1 == N) V1_pp = 0.0;
            else V1_pp = E1[i + 1][j + 1];

            if (i + 1 == N) V1_p = V_val;
            else V1_p = E1[i + 1][j];

            if (j + 1 == N) V1_m = 0.0;
            else V1_m = E1[i][j + 1];

            E1[i][j] = (p1 * p2 * V1_pp + p1 * (1.0 - p2) * V1_p + (1.0 - p1) * p2 * V1_m - b1_opt) / den;

            double V2_pp, V2_p, V2_m;
            if (i + 1 == N && j + 1 == N) V2_pp = V_val / 2.0;
            else if (j + 1 == N) V2_pp = V_val;
            else if (i + 1 == N) V2_pp = 0.0;
            else V2_pp = E2[i + 1][j + 1];

            if (j + 1 == N) V2_p = V_val;
            else V2_p = E2[i][j + 1];

            if (i + 1 == N) V2_m = 0.0;
            else V2_m = E2[i + 1][j];

            E2[i][j] = (p1 * p2 * V2_pp + (1.0 - p1) * p2 * V2_m + p1 * (1.0 - p2) * V2_p - b2_opt) / den;

            // Probability of winning
            double W1_pp, W1_p, W1_m;
            if (i + 1 == N && j + 1 == N) W1_pp = 0.5;
            else if (i + 1 == N) W1_pp = 1.0;
            else if (j + 1 == N) W1_pp = 0.0;
            else W1_pp = W1[i + 1][j + 1];

            if (i + 1 == N) W1_p = 1.0;
            else W1_p = W1[i + 1][j];

            if (j + 1 == N) W1_m = 0.0;
            else W1_m = W1[i][j + 1];

            W1[i][j] = (p1 * p2 * W1_pp + p1 * (1.0 - p2) * W1_p + (1.0 - p1) * p2 * W1_m) / den;
        }
    }

    cout << fixed << setprecision(10) << W1[0][0] << " " << 1.0 - W1[0][0] << endl;

    return 0;
}

