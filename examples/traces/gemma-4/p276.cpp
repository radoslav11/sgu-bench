#include <iostream>

/**
 * Problem Summary:
 * Andrew's training starts at time S and he arrives at time P.
 * Lateness (L) is defined as P - S.
 * The rules for tea owing based on lateness (in seconds):
 * - If L <= 0: 0 cups of tea.
 * - If 1 <= L < 300 (5 minutes): 1 cup of tea.
 * - If 300 <= L < 900 (15 minutes): 2 cups of tea.
 * - If 900 <= L < 1800 (30 minutes): 3 cups of tea.
 * - If L >= 1800 (30 minutes or more): 4 cups of tea.
 *
 * Input constraints:
 * 0 <= S, P <= 10^4.
 * 
 * Sample check:
 * Test #1: S=10, P=10 => L=0, Output: 0. Correct.
 * Test #2: S=10, P=11 => L=1, Output: 1. Correct.
 * Test #3: S=0, P=300 => L=300, Output: 2. Correct.
 */

int main() {
    // Faster I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int S, P;
    // The problem states the input file contains a single line with two integers.
    if (!(std::cin >> S >> P)) {
        return 0;
    }

    int L = P - S;

    // If he is not late or arrives exactly at the appointed time, he owes 0 cups.
    if (L <= 0) {
        std::cout << 0 << std::endl;
    } 
    // If he is late by at least 1 second but less than 5 minutes (300 seconds).
    else if (L < 300) {
        std::cout << 1 << std::endl;
    } 
    // If he is late by at least 5 minutes (300 seconds) but less than 15 minutes (900 seconds).
    else if (L < 900) {
        std::cout << 2 << std::endl;
    } 
    // If he is late by at least 15 minutes (900 seconds) but less than 30 minutes (1800 seconds).
    else if (L < 1800) {
        std::cout << 3 << std::endl;
    } 
    // If he is late by 30 minutes (1800 seconds) or more.
    else {
        std::cout << 4 << std::endl;
    }

    return 0;
}

