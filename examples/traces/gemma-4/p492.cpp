#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

/**
 * Problem: Find the N-th positive integer that does not contain the substring "13".
 * N can be as large as 10^18.
 * 
 * Solution Approach:
 * Since the function f(X), which counts the number of positive integers in [1, X] 
 * not containing "13", is monotonically increasing, we can use binary search.
 * To compute f(X), we use Digit Dynamic Programming.
 * 
 * Complexity:
 * T = 100 test cases.
 * Binary search range [1, 8e18] takes about 63 steps.
 * Digit DP state: pos (20) * is_less (2) * last_was_1 (2).
 * Total complexity: 100 * 63 * 20 * 2 * 2 * 10 = 5 * 10^6, fits in 0.25s.
 */

typedef unsigned long long ull;

ull memo[20][2][2];
int visited[20][2][2];
int timer = 0;
int digits[20];
int num_digits;

/**
 * solve(pos, is_less, last_was_1)
 * pos: Current digit position being processed.
 * is_less: Boolean, true if the number formed so far is already strictly less than the prefix of X.
 * last_was_1: Boolean, true if the previously placed digit was '1'.
 * 
 * Returns the count of ways to complete the number without forming "13".
 */
ull solve(int pos, bool is_less, bool last_was_1) {
    if (pos == num_digits) return 1;
    if (visited[pos][is_less][last_was_1] == timer) return memo[pos][is_less][last_was_1];

    ull count = 0;
    int limit = is_less ? 9 : digits[pos];

    for (int d = 0; d <= limit; ++d) {
        if (last_was_1 && d == 3) continue;
        count += solve(pos + 1, is_less || (d < limit), d == 1);
    }
    
    visited[pos][is_less][last_was_1] = timer;
    memo[pos][is_less][last_was_1] = count;
    return count;
}

/**
 * count_without_13(X)
 * Returns the count of integers in [0, X] that do not contain "13".
 */
ull count_without_13(ull X) {
    if (X == 0) return 1;
    num_digits = 0;
    ull tempX = X;
    int temp_digits[20];
    int k = 0;
    while (tempX > 0) {
        temp_digits[k++] = tempX % 10;
        tempX /= 10;
    }
    num_digits = k;
    for (int i = 0; i < num_digits; ++i) {
        digits[i] = temp_digits[num_digits - 1 - i];
    }
    
    timer++; // Increment timer to avoid repeated memset
    return solve(0, false, false);
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        ull N;
        if (!(cin >> N)) break;

        // Binary search for the smallest X such that f(X) >= N, where f(X) = count_without_13(X) - 1.
        // The N-th number will be around N * (10/9.899) because the density of numbers 
        // not containing "13" is approx 0.9899. For N=10^18, X is around 1.01e18.
        // 8e18 is a safe upper bound within unsigned long long's range.
        ull low = 1, high = 8000000000000000000ULL;
        ull ans = high;

        while (low <= high) {
            ull mid = low + (high - low) / 2;
            // count_without_13(mid) includes the number 0. 
            // The number of valid positive integers in [1, mid] is count_without_13(mid) - 1.
            if (count_without_13(mid) - 1 >= N) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}

