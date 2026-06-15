#include <iostream>
#include <iomanip>

using namespace std;

/**
 * Problem Analysis:
 * The problem asks us to find the fixed monthly payment x for a loan of s burles
 * over m months with a monthly interest rate of p percent. 
 * 
 * Each month:
 * 1. Interest part (a_i): a_i = current_debt * (p / 100)
 * 2. Repayment part (b_i): b_i = x - a_i
 * 3. New debt: new_debt = current_debt - b_i
 * 
 * Simplifying the new debt:
 * new_debt = current_debt - (x - current_debt * (p / 100))
 * new_debt = current_debt + current_debt * (p / 100) - x
 * new_debt = current_debt * (1 + p/100) - x
 * 
 * The total debt must be exactly 0 after m months.
 * Since the final remaining debt is a monotonically decreasing function of x,
 * we can use binary search to find the appropriate value for x.
 * 
 * Constraints:
 * 1 <= s <= 10^6
 * 1 <= m <= 120
 * 0 <= p <= 100
 * 
 * Maximum value of x:
 * When m = 1, x = s * (1 + p/100).
 * Given s = 10^6 and p = 100, the maximum x is 10^6 * (1 + 1) = 2,000,000.
 * So, a safe upper bound for binary search is 2e6 or 1e9.
 * 
 * Precision:
 * We need a precision of 10^-5. Using 100 iterations of binary search
 * with a double-precision floating-point number will provide much higher precision.
 */

// check function to determine if a given monthly payment x is sufficient
// to clear the debt s within m months with interest rate p%
bool check(double x, double s, int m, double p) {
    double current_debt = s;
    double r = p / 100.0;
    for (int i = 0; i < m; ++i) {
        // Update debt for each month based on the derived formula
        current_debt = current_debt * (1.0 + r) - x;
        // In cases where the debt becomes very large and negative (if x is large),
        // we don't need to continue because we only care if current_debt <= 0.
        // However, the constraints allow double to handle very large numbers,
        // and binary search will naturally move towards the correct x.
    }
    return current_debt <= 0;
}

int main() {
    // Faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double s, p;
    int m;

    // Read input values
    if (!(cin >> s >> m >> p)) {
        return 0;
    }

    // Binary search bounds
    double low = 0.0;
    double high = 2000000.0; 
    // To be extra safe, we could set high to 1e9, but 2e6 is theoretically enough.
    // Let's adjust slightly to 1e9 just to avoid any edge cases.
    high = 1000000000.0; 

    // Execute binary search for 100 iterations to ensure high precision
    for (int i = 0; i < 100; ++i) {
        double mid = low + (high - low) / 2.0;
        if (check(mid, s, m, p)) {
            // If mid is sufficient, try a smaller payment
            high = mid;
        } else {
            // If mid is not sufficient, we need a larger payment
            low = mid;
        }
    }

    // Output the result with required precision
    cout << fixed << setprecision(10) << high << endl;

    return 0;
}

