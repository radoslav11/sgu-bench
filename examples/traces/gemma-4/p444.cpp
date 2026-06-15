#include <iostream>
#include <vector>

/**
 * Problem Analysis:
 * The goal is to find the structure of the decimal expansion of the fraction x/y.
 * Specifically, we need to find the number of digits after the decimal point before
 * the decimal expansion becomes periodic (the recurring part) and the length
 * of that recurring part. If the fraction is a finite decimal, the recurring 
 * part length should be 0.
 * 
 * Long division approach:
 * To find the decimal expansion of x/y:
 * 1. Find the initial remainder r = x % y.
 * 2. Perform long division by repeatedly multiplying r by 10 and taking the quotient
 *    and remainder.
 * 3. The sequence of remainders (r) dictates the sequence of decimal digits.
 * 4. A decimal expansion is finite if the remainder becomes 0.
 * 5. A decimal expansion is periodic if a remainder repeats.
 * 
 * Data Structures:
 * Since y can be up to 1,000,000, we can use an array or vector to store the position
 * (index in the decimal expansion) where each remainder was first encountered.
 * This allows us to detect the beginning of the recurring part in O(1) time.
 * 
 * Complexity:
 * Time Complexity: O(y), as we will visit at most y distinct remainders.
 * Space Complexity: O(y) to store the positions of remainders in an array.
 */

using namespace std;

int main() {
    // Faster I/O for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int x, y;
    // Read the dividend and divisor
    if (!(cin >> x >> y)) return 0;

    // The integer part of the division (x/y) does not affect the digits
    // after the decimal point. We only care about the initial remainder.
    int r = x % y;

    // Case 1: The division results in an integer (no digits after decimal point).
    if (r == 0) {
        cout << "0 0" << endl;
        return 0;
    }

    // pos[rem] will store the count of digits seen after the decimal point
    // at the time the remainder 'rem' was first encountered.
    // Initialize all positions to -1 to indicate they haven't been seen yet.
    // Given y <= 1,000,000, the maximum possible remainder is 999,999.
    vector<int> pos(y, -1);

    int count = 0; // Current number of digits after the decimal point.
    
    while (r != 0) {
        // Case 2: If the current remainder has been seen before, a loop is detected.
        if (pos[r] != -1) {
            // The number of digits before the loop is the position where the 
            // remainder was first seen.
            int before_loop = pos[r];
            // The length of the recurring part is the current digit count minus
            // the original position.
            int loop_length = count - pos[r];
            cout << before_loop << " " << loop_length << endl;
            return 0;
        }

        // Mark the current remainder's position.
        pos[r] = count;

        // Calculate the next remainder after multiplying by 10 (standard long division).
        // Note: r * 10 can be up to ~10,000,000, which fits in a 32-bit int.
        r = (r * 10) % y;
        
        // Move to the next decimal position.
        count++;
    }

    // Case 3: If r becomes 0, the decimal expansion is finite.
    // The number of digits after the decimal point is 'count', and the loop length is 0.
    cout << count << " 0" << endl;

    return 0;
}

