#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: 546. Ternary Password
 * We want to transform a ternary password (0, 1, 2) of length n to contain 
 * exactly a '0's, b '1's, and (n - a - b) '2's with minimum replacements.
 * 
 * Strategy:
 * 1. Check if a + b > n. If so, output -1.
 * 2. Count current '0's, '1's, and '2's.
 * 3. Greedily adjust the counts to reach the target counts:
 *    a) First, adjust the count of '0's.
 *       If we need more '0's, prioritize changing '1's that are in excess of b, 
 *       then change '2's.
 *       If we have too many '0's, prioritize changing '0's to '1's (if '1's are needed),
 *       then to '2's.
 *    b) After fixing '0's, adjust the count of '1's.
 *       If we need more '1's, change '2's to '1's.
 *       If we have too many '1's, change '1's to '2's.
 *    c) The count of '2's will naturally be corrected.
 */

int main() {
    // Optimization for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, a, b;
    if (!(cin >> n >> a >> b)) return 0;
    string S;
    cin >> S;

    // Target count for '2's
    int c = n - a - b;

    // If the required number of '0's and '1's exceeds the password length
    if (a + b > n) {
        cout << -1 << endl;
        return 0;
    }

    int c0 = 0, c1 = 0, c2 = 0;
    for (char ch : S) {
        if (ch == '0') c0++;
        else if (ch == '1') c1++;
        else c2++;
    }

    string ans_string = S;
    int replacements = 0;

    // Step 1: Adjust '0's count
    if (c0 < a) {
        int need = a - c0;
        // Priority: change '1's that are excess to '0'
        for (int i = 0; i < n && need > 0; ++i) {
            if (S[i] == '1' && c1 > b) {
                ans_string[i] = '0';
                c1--; c0++; need--; replacements++;
            }
        }
        // Next: change '2's to '0'
        for (int i = 0; i < n && need > 0; ++i) {
            if (S[i] == '2') {
                ans_string[i] = '0';
                c2--; c0++; need--; replacements++;
            }
        }
    } else if (c0 > a) {
        int excess = c0 - a;
        // Priority: change '0's to '1's that are needed
        for (int i = 0; i < n && excess > 0; ++i) {
            if (S[i] == '0' && c1 < b) {
                ans_string[i] = '1';
                c0--; c1++; excess--; replacements++;
            }
        }
        // Next: change '0's to '2's
        for (int i = 0; i < n && excess > 0; ++i) {
            if (S[i] == '0') {
                ans_string[i] = '2';
                c0--; c2++; excess--; replacements++;
            }
        }
    }

    // Step 2: Adjust '1's count (c0 is now correctly 'a')
    if (c1 < b) {
        int need = b - c1;
        // We must take from '2's because c0=a and c1 < b implies c2 > c
        for (int i = 0; i < n && need > 0; ++i) {
            if (S[i] == '2') {
                ans_string[i] = '1';
                c2--; c1++; need--; replacements++;
            }
        }
    } else if (c1 > b) {
        int excess = c1 - b;
        // We change '1's to '2's
        for (int i = 0; i < n && excess > 0; ++i) {
            if (S[i] == '1') {
                ans_string[i] = '2';
                c1--; c2++; excess--; replacements++;
            }
        }
    }

    // Final verification: Since total length is constant, if a and b are correct, 
    // then c (the count of 2s) is also correct.

    cout << replacements << endl;
    cout << ans_string << endl;

    return 0;
}

