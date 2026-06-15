#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    
    std::vector<int> votes(n);
    long long total = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> votes[i];
        total += votes[i];
    }
    
    // Calculate floor and ceiling for each candidate
    std::vector<int> floor_vals(n), ceil_vals(n);
    for (int i = 0; i < n; i++) {
        // Exact percentage: (votes[i] * 100.0) / total
        floor_vals[i] = static_cast<int>(std::floor(static_cast<double>(votes[i]) * 100.0 / total));
        ceil_vals[i] = static_cast<int>(std::ceil(static_cast<double>(votes[i]) * 100.0 / total));
    }
    
    // Calculate minimum and maximum possible sum
    long long min_sum = 0, max_sum = 0;
    for (int i = 0; i < n; i++) {
        min_sum += floor_vals[i];
        max_sum += ceil_vals[i];
    }
    
    // Check if 100 is within the range [min_sum, max_sum]
    if (100 < min_sum || 100 > max_sum) {
        std::cout << "No solution" << std::endl;
        return 0;
    }
    
    // Start with all floor values
    std::vector<int> result = floor_vals;
    long long current_sum = min_sum;
    
    // Calculate how many increments we need
    int needed = 100 - current_sum;
    
    // For each candidate, calculate the "remainder" for greedy selection
    // We want to prioritize candidates where the fractional part is largest
    // because they are closest to the next integer (ceiling)
    std::vector<std::pair<double, int>> fractional_parts;
    for (int i = 0; i < n; i++) {
        double exact = static_cast<double>(votes[i]) * 100.0 / total;
        double frac = exact - floor_vals[i];
        fractional_parts.push_back({-frac, i}); // negative for min-heap behavior with sorted
    }
    
    // Sort by fractional part descending (using negative for ascending sort)
    std::sort(fractional_parts.begin(), fractional_parts.end());
    
    // Add increments to candidates with largest fractional parts first
    for (int i = 0; i < needed; i++) {
        int idx = fractional_parts[i].second;
        result[idx]++;
    }
    
    // Output the result
    for (int i = 0; i < n; i++) {
        if (i > 0) std::cout << " ";
        std::cout << result[i];
    }
    std::cout << std::endl;
    
    return 0;
}
