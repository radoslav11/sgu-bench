#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

// Function to factorize n into prime factors
// Returns a map of prime -> exponent
map<long long, int> factorize(long long n) {
    map<long long, int> factors;
    if (n <= 1) {
        return factors;
    }
    
    // Check for factor 2
    while (n % 2 == 0) {
        factors[2]++;
        n /= 2;
    }
    
    // Check for odd factors from 3 to sqrt(n)
    for (long long i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors[i]++;
            n /= i;
        }
    }
    
    // If n is still greater than 1, then it's a prime factor
    if (n > 1) {
        factors[n]++;
    }
    
    return factors;
}

// Function to compute the number of partitions of k
// This counts the number of ways to write k as a sum of positive integers
// where order doesn't matter (partition function p(k))
long long partition(int k) {
    if (k == 0) return 1;
    if (k == 1) return 1;
    
    // Use pentagonal number theorem for partition function
    vector<long long> p(k + 1, 0);
    p[0] = 1;
    
    for (int i = 1; i <= k; i++) {
        p[i] = 0;
        for (int j = 1; ; j++) {
            // Generalized pentagonal numbers: g(k) = k(3k-1)/2
            int k1 = j;
            int g1 = k1 * (3 * k1 - 1) / 2;
            if (g1 > i) break;
            
            int sign = (j % 2 == 0) ? -1 : 1;
            p[i] += sign * p[i - g1];
            
            int g2 = k1 * (3 * k1 + 1) / 2;
            if (g2 > i) break;
            p[i] += sign * p[i - g2];
        }
        // Handle negative result (shouldn't happen with correct algorithm)
        if (p[i] < 0) p[i] = -p[i];
    }
    
    return p[k];
}

int main() {
    long long n;
    cin >> n;
    
    // Special case: n = 1
    if (n == 1) {
        cout << 1 << endl;
        return 0;
    }
    
    // Factorize n
    map<long long, int> factors = factorize(n);
    
    long long result = 1;
    
    // For each prime factor with exponent e, 
    // the number of abelian groups is the number of partitions of e
    // Because the fundamental theorem says the group is a direct sum of cyclic groups of prime power order
    // and for each prime p, the p-Sylow subgroup is determined by a partition of the exponent
    for (auto& [prime, exp] : factors) {
        long long partitions = partition(exp);
        result *= partitions;
    }
    
    cout << result << endl;
    
    return 0;
}
