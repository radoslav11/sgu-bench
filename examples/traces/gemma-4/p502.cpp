#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// dp[mask] stores a bitmask of possible remainders modulo 17 for a given subset of digits.
// The i-th bit of dp[mask] is 1 if it's possible to form a number with digits in 'mask'
// such that the number modulo 17 is i.
uint16_t dp[1 << 18];

// parent[mask][rem] stores the index of the digit that was used to reach the remainder 'rem'
// for the given subset of digits 'mask'.
signed char parent[1 << 18][17];

int d[18];
int next_rem_table[17][10];

int main() {
    // Speed up input and output.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int N = s.length();
    for (int i = 0; i < N; ++i) d[i] = s[i] - '0';

    // Precompute the next remainder table for all (current_remainder, next_digit) pairs.
    for (int rem = 0; rem < 17; ++rem) {
        for (int digit = 0; digit < 10; ++digit) {
            next_rem_table[rem][digit] = (rem * 10 + digit) % 17;
        }
    }

    // Initial step: set the possible remainders for each single digit (mask with 1 bit set).
    // The first digit of the number cannot be zero.
    for (int i = 0; i < N; ++i) {
        if (d[i] != 0) {
            int rem = d[i] % 17;
            if (!(dp[1 << i] & (1 << rem))) {
                dp[1 << i] |= (1 << rem);
                parent[1 << i][rem] = (signed char)i;
            }
        }
    }

    // Fill the DP table. We iterate through each mask from 1 to (2^N - 1).
    for (int mask = 1; mask < (1 << N); ++mask) {
        uint16_t rems = dp[mask];
        if (rems == 0) continue; // Skip if this subset of digits can't form any number.
        
        // Find which digits are not yet included in the current mask.
        int remaining_bits = ((1 << N) - 1) ^ mask;
        while (remaining_bits) {
            // Get the index of the next available digit using built-in instruction.
            int i = __builtin_ctz(remaining_bits);
            int next_mask = mask | (1 << i);
            int digit = d[i];
            
            // For each possible remainder currently achievable with 'mask',
            // calculate the new remainder after adding the current digit.
            uint16_t temp_rems = rems;
            while (temp_rems) {
                int rem = __builtin_ctz(temp_rems);
                int next_rem = next_rem_table[rem][digit];
                // If this new remainder has not been reached for 'next_mask' yet, update dp and parent.
                if (!(dp[next_mask] & (1 << next_rem))) {
                    dp[next_mask] |= (1 << next_rem);
                    parent[next_mask][next_rem] = (signed char)i;
                }
                temp_rems &= (temp_rems - 1); // Clear the lowest set bit.
            }
            remaining_bits &= (remaining_bits - 1); // Clear the lowest set bit.
        }
    }

    // Check if any permutation of all digits (full mask) results in a remainder of 0.
    int full_mask = (1 << N) - 1;
    if (dp[full_mask] & (1 << 0)) {
        int cur_mask = full_mask;
        int cur_rem = 0;
        string res = "";
        // Reconstruct the number by backtracking through the parent pointers.
        while (cur_mask > 0) {
            int i = parent[cur_mask][cur_rem];
            res += (char)(d[i] + '0');
            int prev_mask = cur_mask ^ (1 << i);
            if (prev_mask == 0) break;
            
            // Backtrack the remainder:
            // We know cur_rem = (prev_rem * 10 + d[i]) % 17.
            // To find prev_rem:
            // prev_rem * 10 ≡ (cur_rem - d[i]) (mod 17).
            // Multiplying both sides by the modular inverse of 10 mod 17 (which is 12):
            // prev_rem ≡ (cur_rem - d[i]) * 12 (mod 17).
            cur_rem = ((cur_rem - d[i]) % 17 + 17) % 17 * 12 % 17;
            cur_mask = prev_mask;
        }
        // The number was reconstructed from most significant digit to least, but
        // our backtrack actually finds digits from least to most in reverse order of placement.
        // Wait, the loop builds the number from last digit back to first.
        // So reverse it at the end.
        reverse(res.begin(), res.end());
        cout << res << endl;
    } else {
        // If no valid permutation was found, output -1.
        cout << -1 << endl;
    }

    return 0;
}

