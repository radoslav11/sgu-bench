#include <iostream>

/**
 * Problem Analysis:
 * The problem asks for an integer number 'n' that is 'x' times less than the sum of its 
 * integer positive predecessors.
 * 
 * Let n be the integer.
 * The positive integer predecessors of n are 1, 2, 3, ..., n-1.
 * The sum of these predecessors (S) is given by the formula for an arithmetic progression:
 * S = (n - 1) * n / 2
 * 
 * The problem states that n is x times less than S, which can be written as:
 * n = S / x 
 * or
 * S = x * n
 * 
 * Substituting the sum formula:
 * (n - 1) * n / 2 = x * n
 * 
 * Since n must be a positive integer (the sum of predecessors implies n > 1), 
 * we can divide both sides by n:
 * (n - 1) / 2 = x
 * n - 1 = 2 * x
 * n = 2 * x + 1
 * 
 * Given that 1 <= x <= 10^9, the result n = 2 * x + 1 will be at most 2,000,000,001.
 * This value fits within a standard 32-bit signed integer (max ~2.147 * 10^9),
 * but using long long ensures no overflow during the calculation.
 */

int main() {
    // Optimize input and output performance
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    long long x;
    // Read the input x from standard input
    if (std::cin >> x) {
        // Calculate n = 2 * x + 1 and print it to standard output
        long long n = 2 * x + 1;
        std::cout << n << std::endl;
    }

    return 0;
}

