#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem: X + R(X) = N, find the number of positive integers X.
 * R(X) is the number X written backwards.
 * N can be up to 100 digits.
 * 
 * Let L be the number of digits in X.
 * X = d_{L-1} * 10^{L-1} + ... + d_0 * 10^0.
 * R(X) = d_0 * 10^{L-1} + ... + d_{L-1} * 10^0.
 * Let s_i = d_i + d_{L-1-i}. Note s_i = s_{L-1-i}.
 * X + R(X) = sum_{i=0}^{L-1} s_i * 10^i = N.
 * 
 * We use digit-by-digit DP (memoization) to solve this.
 * The state is solve(index, carry_low, carry_high), where:
 * - index is the current position being processed (from 0 up to (L-1)/2).
 * - carry_low is the carry into position 'index' from 'index-1'.
 * - carry_high is the carry into position 'L-index' from 'L-index-1'.
 */

typedef long long ll;

string N_str;
vector<int> n_digits;
int M;
ll memo[105][2][2];
int current_L;

// Number of pairs (d_i, d_{L-1-i}) such that d_i + d_{L-1-i} = s, with constraints.
ll count_pairs(int s, int i, int L) {
    if (s < 0 || s > 18) return 0;
    if (i == 0) {
        // d_{L-1} must be in [1, 9], d_0 in [0, 9]
        // d_0 = s - d_{L-1}. Since 1 <= d_{L-1} <= 9 and 0 <= d_0 <= 9:
        // d_{L-1} can be any value such that 1 <= d_{L-1} <= 9 and 0 <= s - d_{L-1} <= 9
        // => s - 9 <= d_{L-1} <= s.
        // Combining: max(1, s-9) <= d_{L-1} <= min(9, s).
        int lower = max(1, s - 9);
        int upper = min(9, s);
        return (lower <= upper) ? (upper - lower + 1) : 0;
    } else {
        // Standard digit pair count for 0 <= d_i, d_{L-1-i} <= 9
        int lower = max(0, s - 9);
        int upper = min(9, s);
        return (lower <= upper) ? (upper - lower + 1) : 0;
    }
}

ll solve(int i, int c_low, int c_high) {
    if (i > (current_L - 1) / 2) {
        return (c_low == c_high) ? 1 : 0;
    }
    if (memo[i][c_low][c_high] != -1) {
        return memo[i][c_low][c_high];
    }

    ll ans = 0;
    if (current_L % 2 != 0 && i == (current_L - 1) / 2) {
        // Middle digit for odd L
        for (int s = 0; s <= 18; s += 2) {
            int d = s / 2;
            if (d >= 0 && d <= 9) {
                int c_next = (s + c_low) / 10;
                if ((s + c_low) % 10 == n_digits[i] && c_next == c_high) {
                    ans++;
                }
            }
        }
    } else {
        // Pairs of digits (d_i, d_{L-1-i})
        for (int s = 0; s <= 18; ++s) {
            int c_next_low = (s + c_low) / 10;
            if ((s + c_low) % 10 == n_digits[i]) {
                // Check the carry from position L-1-i into L-i
                // s + c_{L-1-i} = n_{L-1-i} + 10 * c_high
                // c_{L-1-i} = n_{L-1-i} + 10 * c_high - s
                int c_next_high = n_digits[current_L - 1 - i] + 10 * c_high - s;
                if (c_next_high == 0 || c_next_high == 1) {
                    ll pairs = count_pairs(s, i, current_L);
                    if (pairs > 0) {
                        ans += pairs * solve(i + 1, c_next_low, c_next_high);
                    }
                }
            }
        }
    }

    return memo[i][c_low][c_high] = ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    while (cin >> N_str && N_str != "0") {
        M = N_str.length();
        n_digits.resize(M);
        for (int i = 0; i < M; ++i) {
            n_digits[i] = N_str[M - 1 - i] - '0';
        }

        ll total_ans = 0;
        // X can have M or M-1 digits.
        for (int L : {M - 1, M}) {
            if (L <= 0) continue;
            current_L = L;
            int c_L = 0;
            if (L < M) {
                if (M - L == 1 && n_digits[M - 1] == 1) {
                    c_L = 1;
                } else {
                    continue;
                }
            } else if (L == M) {
                c_L = 0;
            }

            memset(memo, -1, sizeof(memo));
            // Initial carry into position 0 is 0, and carry into position L (from L-1) is c_L.
            total_ans += solve(0, 0, c_L);
        }

        cout << total_ans << "\n";
    }

    return 0;
}

