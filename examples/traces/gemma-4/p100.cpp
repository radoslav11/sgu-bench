#include <iostream>

/**
 * Problem: A+B
 * Description: Read two integers A and B and output their sum.
 * Constraints: 0 < A, B < 10001
 * Time Limit: 0.25 sec
 * Memory Limit: 65536 KB
 * 
 * The constraints are small, so standard integer types and 
 * standard input/output streams are sufficient.
 */

int main() {
    // Speed up C++ standard streams for competitive programming.
    // This is a good practice, especially when the time limit is tight.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int a, b;

    // Using while(std::cin >> a >> b) handles cases where there might be
    // multiple pairs of integers in the input, although the problem 
    // description suggests a single pair. This is a robust way to read.
    while (std::cin >> a >> b) {
        // Compute and output the sum followed by a newline.
        // Using "\n" instead of std::endl to avoid unnecessary stream flushing.
        std::cout << (a + b) << "\n";
    }

    return 0;
}

