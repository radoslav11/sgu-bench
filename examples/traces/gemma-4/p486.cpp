#include <iostream>
#include <string>

/**
 * Problem: Bulls and Cows
 * 
 * A "bull" is a digit that is present in both the secret and the guess 
 * and occurs in the same position in both.
 * A "cow" is a digit that is present in both numbers, but occurs in 
 * different positions.
 * 
 * Given that all digits in the secret and the guess are distinct, 
 * a digit can either be a bull or a cow, but not both.
 * 
 * Complexity: 
 * - Time: O(1) because the number of digits is always 4.
 * - Space: O(1) to store the input.
 */

using namespace std;

int main() {
    // Use strings to handle the 4-digit numbers, which allows leading zeros.
    string secret, guess;

    // Read the secret number and the guessed number from standard input.
    if (!(cin >> secret >> guess)) {
        return 0;
    }

    int bulls = 0;
    int cows = 0;

    // Count bulls: Compare digits at the same position.
    for (int i = 0; i < 4; ++i) {
        if (secret[i] == guess[i]) {
            bulls++;
        }
    }

    // Count cows: Compare digits at different positions.
    // Since digits are distinct within each number, 
    // we check if secret[i] matches guess[j] where i != j.
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // If they are at different indices and the digits match, it's a cow.
            if (i != j && secret[i] == guess[j]) {
                cows++;
            }
        }
    }

    // Output the results separated by a space.
    cout << bulls << " " << cows << endl;

    return 0;
}

