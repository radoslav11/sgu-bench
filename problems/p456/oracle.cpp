#include <bits/stdc++.h>
using namespace std;

// Compute the monthly payment for an annuity loan.
// s: initial debt (principal), m: number of months, p: interest rate (percent per month)
double findMonthlyPayment(double s, int m, double p) {
    // If interest rate is zero, it's just principal divided evenly
    if (p == 0.0) {
        return s / m;
    }
    // Convert percent to decimal
    double r = p / 100.0;  
    // Compute (1 + r)^m
    double factor = pow(1.0 + r, m);
    // Numerator: r * (1 + r)^m
    double numerator = r * factor;
    // Denominator: (1 + r)^m - 1
    double denominator = factor - 1.0;
    // Annuity formula: s * [r*(1+r)^m] / [(1+r)^m - 1]
    return s * numerator / denominator;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read input: s (principal), m (months), p (percent interest)
    double s;
    int m;
    double p;
    cin >> s >> m >> p;

    // Compute the monthly payment
    double x = findMonthlyPayment(s, m, p);

    // Print with exactly 5 digits after decimal point
    cout << fixed << setprecision(5) << x << "\n";
    return 0;
}
