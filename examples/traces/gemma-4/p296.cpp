#include <iostream>
#include <string>
#include <vector>

/**
 * Problem: Sasha vs. Kate
 * Task: Given a number N (up to 1000 digits) and an integer K, remove exactly K decimal digits 
 * from N to make the resulting number P as large as possible.
 * 
 * Approach:
 * This is a classic greedy problem that can be solved using a monotonic stack.
 * We want to keep the largest possible digits at the most significant positions (leftmost).
 * 
 * Algorithm:
 * 1. Traverse the digits of N from left to right.
 * 2. For each digit, while the current digit is greater than the top of the stack and we 
 *    still have K digits left to remove, pop from the stack and decrement K.
 * 3. Push the current digit onto the stack.
 * 4. After processing all digits of N, if K is still greater than zero, pop the 
 *    remaining K digits from the top of the stack (the end of the stack).
 * 5. The digits remaining in the stack represent the maximal number P.
 * 
 * Complexity:
 * - Time complexity: O(|N|), where |N| is the number of digits in N. Each digit is pushed 
 *   and popped from the stack at most once.
 * - Space complexity: O(|N|) to store the digits in the stack.
 * 
 * Constraints check:
 * - N can have up to 1001 digits (for N = 10^1000).
 * - K can be up to 999.
 * - K is always strictly less than the number of digits in N.
 * - Time limit (0.5s) and memory limit (64MB) are more than enough for O(|N|).
 */

using namespace std;

int main() {
    // Optimize standard I/O performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string N;
    int K;
    // Read N as a string and K as an integer
    if (!(cin >> N >> K)) {
        return 0;
    }

    // Using a string as a stack to store the digits of the resulting number
    string result = "";
    int digitsToRemove = K;

    for (char digit : N) {
        // While we still have digits to remove and the current digit is greater 
        // than the last digit in our result, we remove the smaller digit.
        while (digitsToRemove > 0 && !result.empty() && digit > result.back()) {
            result.pop_back();
            digitsToRemove--;
        }
        result.push_back(digit);
    }

    // If we still need to remove more digits (in cases where N was non-increasing),
    // remove the required number from the end of the result.
    while (digitsToRemove > 0 && !result.empty()) {
        result.pop_back();
        digitsToRemove--;
    }

    // The result is the maximal number P.
    cout << result << endl;

    return 0;
}

