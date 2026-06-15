#include <iostream>

/**
 * Problem Analysis:
 * A standard die has 6 faces where the sum of opposite faces is 7.
 * The sum of all six faces is 1+2+3+4+5+6 = 21.
 * The sum of the four side faces of any die is 21 - 7 = 14.
 * 
 * A tower of k dice has:
 * - k sets of 4 side faces.
 * - One top face of the top die (die k).
 * - One bottom face of the bottom die (die 1).
 * 
 * Let B1 be the bottom face of die 1 and Tk be the top face of die k.
 * Sum n = 14 * k + B1 + Tk.
 * 
 * Case k = 1:
 * The top and bottom faces are opposite faces of the same die.
 * Therefore, B1 + T1 must be 7.
 * n = 14 * 1 + 7 = 21.
 * 
 * Case k >= 2:
 * The top face of the top die (Tk) and the bottom face of the bottom die (B1)
 * can be any value from 1 to 6 because they belong to different dice.
 * Thus, the sum B1 + Tk can be any value from (1+1) = 2 to (6+6) = 12.
 * n = 14 * k + rem, where rem is in [2, 12].
 * 
 * Combined:
 * n = 21 is the only case for k = 1.
 * For k >= 2, n must satisfy n % 14 is in [2, 12] and k = n / 14 >= 2.
 * 
 * We want to find the minimum k. Since the ranges [14k + 2, 14k + 12] for k >= 2
 * are disjoint and do not include 21, each valid n corresponds to at most one k.
 */

using namespace std;

int main() {
    // Optimize input/output operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    if (!(cin >> n)) return 0;

    // Check for the only possible k=1 case
    if (n == 21) {
        cout << 1 << endl;
    } 
    // Check for k >= 2 cases
    else if (n >= 30) {
        long long k = n / 14;
        long long rem = n % 14;
        // n = 14*k + rem where rem must be in [2, 12] for k >= 2
        if (rem >= 2 && rem <= 12) {
            cout << k << endl;
        } else {
            cout << -1 << endl;
        }
    } 
    // Any other n is impossible
    else {
        cout << -1 << endl;
    }

    return 0;
}

