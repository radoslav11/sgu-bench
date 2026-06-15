#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<int> getPrimeFactors(int n) {
    std::vector<int> factors;
    
    // Check for factor 2
    if (n % 2 == 0) {
        factors.push_back(2);
        while (n % 2 == 0) {
            n /= 2;
        }
    }
    
    // Check for odd factors from 3 to sqrt(n)
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            factors.push_back(i);
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    
    // If n is still greater than 1, then it's a prime factor
    if (n > 1) {
        factors.push_back(n);
    }
    
    return factors;
}

int calculateKeyNumber(int n) {
    std::vector<int> factors = getPrimeFactors(n);
    
    if (factors.empty()) {
        return 0; // This shouldn't happen for n >= 2
    }
    
    // Find the maximum prime factor
    int maxFactor = *std::max_element(factors.begin(), factors.end());
    
    // Calculate sum of all factors
    int sumAll = 0;
    for (int factor : factors) {
        sumAll += factor;
    }
    
    // Key number = max factor - (sum of all factors - max factor)
    // = max factor - sum of other factors
    return maxFactor - (sumAll - maxFactor);
}

int main() {
    int a, b;
    std::cin >> a >> b;
    
    int keyA = calculateKeyNumber(a);
    int keyB = calculateKeyNumber(b);
    
    if (keyA > keyB) {
        std::cout << "a" << std::endl;
    } else {
        std::cout << "b" << std::endl;
    }
    
    return 0;
}
