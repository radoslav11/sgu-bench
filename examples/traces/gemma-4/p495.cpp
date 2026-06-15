#include <iostream>
#include <cmath>
#include <iomanip>

/**
 * Problem Analysis:
 * We need to find the expected number of distinct boxes selected when M winners pick one of N boxes at random.
 * Let X_i be an indicator variable that is 1 if box i is selected at least once, and 0 otherwise.
 * The number of prizes is X = sum_{i=1 to N} X_i.
 * By linearity of expectation, E[X] = sum_{i=1 to N} E[X_i] = sum_{i=1 to N} P(X_i = 1).
 * P(X_i = 1) = 1 - P(X_i = 0), where P(X_i = 0) is the probability that box i is never picked.
 * Since each of the M winners picks a box with probability (N-1)/N of not picking box i,
 * P(X_i = 0) = ((N-1)/N)^M.
 * Therefore, E[X] = N * (1 - ((N-1)/N)^M).
 * 
 * Precision Consideration:
 * For large N, (N-1)/N is very close to 1. Direct computation using pow((N-1)/N, M)
 * might suffer from precision loss, particularly when N is large or when (N-1)/N is near 1.
 * We can rewrite 1 - ((N-1)/N)^M as:
 * 1 - exp(M * log((N-1)/N))
 * 1 - exp(M * log(1 - 1/N))
 * 
 * To handle cases where 1/N is very small, we can use:
 * 1 - exp(M * log1p(-1/N))
 * which is equal to -(exp(M * log1p(-1/N)) - 1) = -expm1(M * log1p(-1/N)).
 * These functions (log1p and expm1) are part of the C++ <cmath> library and are
 * specifically designed to maintain high precision for small inputs.
 */

using namespace std;

int main() {
    // Optimize I/O performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Use long double for higher precision during intermediate calculations.
    long double N, M;
    if (!(cin >> N >> M)) return 0;

    // If N is 0, the expected number of prizes is 0.
    // However, based on the problem context, N and M should be at least 1.
    if (N < 1.0L) {
        cout << fixed << setprecision(10) << 0.0L << endl;
        return 0;
    }

    // We calculate N * (-expm1(M * log1p(-1.0 / N)))
    // This formula is mathematically equivalent to N * (1 - (1 - 1/N)^M)
    // but is numerically more stable for large N or small 1/N.
    long double ans = N * (-std::expm1(M * std::log1p(-1.0L / N)));

    // Output the result with the required precision.
    cout << fixed << setprecision(10) << ans << endl;

    return 0;
}

