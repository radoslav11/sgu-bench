#include <iostream>
#include <algorithm>

/**
 * Problem: 184. Patties
 * The goal is to find the maximum number of cabbage patties Petya can cook
 * given available amounts of flour (P), milk (M), and cabbage (C), and the 
 * amount required for one patty: flour (K), milk (R), and cabbage (V).
 *
 * The number of patties is limited by the ingredient that runs out first.
 * For each ingredient, the maximum number of patties possible is the 
 * integer division of the available amount by the amount required per patty.
 * The overall maximum number of patties is the minimum of these three values.
 *
 * Constraints: 1 <= P, M, C, K, R, V <= 10000
 * Since the constraints are small, standard integer types and O(1) 
 * arithmetic will easily pass the time and memory limits.
 */

int main() {
    // Speed up standard I/O operations
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int P, M, C, K, R, V;

    // Read the input values until the end of the file is reached.
    // The problem description says "the input file contains integer numbers...",
    // implying a single set of inputs, but using a while loop ensures
    // robust handling for both single and multiple test cases.
    while (std::cin >> P >> M >> C >> K >> R >> V) {
        // Calculate the maximum number of patties that can be made 
        // based on each ingredient individually.
        // Integer division in C++ automatically performs the floor operation.
        int patties_from_flour = P / K;
        int patties_from_milk = M / R;
        int patties_from_cabbage = C / V;

        // The actual number of patties that can be made is the minimum 
        // of the three individual limits.
        // std::min({a, b, c}) is a C++11 feature (using initializer lists)
        // which is perfectly valid in C++17.
        int max_patties = std::min({patties_from_flour, patties_from_milk, patties_from_cabbage});

        // Output the result to standard output
        std::cout << max_patties << "\n";
    }

    return 0;
}

