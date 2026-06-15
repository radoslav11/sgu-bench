#include <cstdio>

/**
 * Problem: Greatest Greatest Common Divisor
 * The goal is to find the maximum possible Greatest Common Divisor (GCD)
 * among all pairs of numbers in a given set of distinct integers.
 * 
 * Approach:
 * Let M be the maximum value of a friendship number (M = 10^6).
 * Instead of iterating over all pairs (which is O(n^2)), we can iterate
 * over all possible GCD values 'd' from M down to 1.
 * For a given 'd', if there are at least two numbers in the input set
 * that are multiples of 'd', then 'd' is a possible GCD.
 * Since we are iterating from largest to smallest, the first 'd' we
 * find that satisfies this condition is the maximum GCD.
 * 
 * Time Complexity:
 * The complexity of this approach is O(M * log M), where M is the maximum
 * friendship number. This is because the inner loop runs M/d times for 
 * each d, and the sum of M/d from d=1 to M is roughly M * ln(M).
 * For M = 10^6, M * ln(M) is approximately 1.4 * 10^7, which fits
 * well within the 0.5-second time limit.
 * 
 * Space Complexity:
 * O(M) to store the presence of each number in an array.
 */

using namespace std;

const int MAXV = 1000000;
// Using a char array to mark presence for efficient memory usage and speed.
char exists[MAXV + 1];

int main() {
    int n;
    // Read the number of people.
    if (scanf("%d", &n) != 1) return 0;

    int max_val = 0;
    for (int i = 0; i < n; ++i) {
        int a;
        // Read each friendship number.
        if (scanf("%d", &a) != 1) break;
        // Mark the number as present in the input set.
        if (a >= 1 && a <= MAXV) {
            exists[a] = 1;
            if (a > max_val) {
                max_val = a;
            }
        }
    }

    // If there are fewer than 2 people, no pairs can be formed.
    // Based on the problem description, we assume n >= 2.
    if (n < 2) {
        return 0;
    }

    // Iterate through all possible GCD values from highest to lowest.
    for (int d = max_val; d >= 1; --d) {
        int count = 0;
        // Check multiples of d: d, 2d, 3d, etc.
        // If we find two or more numbers in the input set that are multiples of d,
        // then d is the maximum possible GCD.
        for (int m = d; m <= max_val; m += d) {
            if (exists[m]) {
                count++;
                if (count >= 2) {
                    printf("%d\n", d);
                    return 0;
                }
            }
        }
    }

    return 0;
}

