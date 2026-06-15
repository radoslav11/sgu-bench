#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * - Starting voltage: 1 Bervolt.
 * - Target voltage: N Bervolts.
 * - Amplifier Type 1: X -> 2X - 1
 * - Amplifier Type 2: X -> 2X + 1
 * 
 * Observations:
 * 1. Both amplifier types produce an odd number from any integer X.
 *    f1(X) = 2X - 1 (always odd)
 *    f2(X) = 2X + 1 (always odd)
 * 2. Since the starting voltage is 1 (odd), all voltages in any sequence will be odd.
 * 3. If N is even, it is impossible to reach N. Output "No solution".
 * 4. To reach N, the previous voltage X must be odd.
 *    If the last amplifier was Type 1: N = 2X - 1 => X = (N + 1) / 2.
 *    For X to be odd, (N + 1) / 2 % 2 == 1 => N + 1 is 2 mod 4 => N % 4 == 1.
 *    If the last amplifier was Type 2: N = 2X + 1 => X = (N - 1) / 2.
 *    For X to be odd, (N - 1) / 2 % 2 == 1 => N - 1 is 2 mod 4 => N % 4 == 3.
 * 5. Because N must be odd, N % 4 must be either 1 or 3. This means at each step,
 *    there is exactly one possible amplifier type that preserves the property
 *    that the preceding voltage was odd.
 * 6. This unique path ensures that any odd N can be reached using the minimum 
 *    number of amplifiers by working backwards from N to 1.
 */

int main() {
    // Optimize I/O operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N;
    // Read target voltage N.
    if (!(cin >> N)) return 0;

    // If target N is even, no sequence of amplifiers can reach it.
    if (N % 2 == 0) {
        cout << "No solution" << endl;
        return 0;
    }

    // Case where N = 1: 0 amplifiers needed.
    if (N == 1) {
        cout << 0 << endl << endl;
        return 0;
    }

    vector<int> res;
    // Work backwards from N to the starting voltage 1.
    while (N > 1) {
        if (N % 4 == 1) {
            // The last amplifier applied was Type 1.
            res.push_back(1);
            N = (N + 1) / 2;
        } else {
            // N % 4 must be 3, so the last amplifier applied was Type 2.
            res.push_back(2);
            N = (N - 1) / 2;
        }
    }

    // The amplifiers were stored in reverse order (from N to 1).
    // Reverse them to output the sequence from 1 to N.
    reverse(res.begin(), res.end());

    // Print the minimal number of amplifiers.
    cout << res.size() << "\n";
    // Print the sequence of amplifier types.
    for (size_t i = 0; i < res.size(); ++i) {
        cout << res[i] << (i == res.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

