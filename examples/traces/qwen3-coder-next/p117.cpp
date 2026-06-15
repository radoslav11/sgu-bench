#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

std::map<long long, int> factorize(long long n) {
    std::map<long long, int> factors;
    if (n <= 1) return factors;
    
    // Check for factor 2
    while (n % 2 == 0) {
        factors[2]++;
        n /= 2;
    }
    
    // Check for odd factors
    for (long long i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            factors[i]++;
            n /= i;
        }
    }
    
    // If n is a prime number greater than 2
    if (n > 1) {
        factors[n]++;
    }
    
    return factors;
}

int main() {
    int N, M, K;
    std::cin >> N >> M >> K;
    
    // Special case: if K == 1, every number works
    if (K == 1) {
        // Read and ignore the sequence
        for (int i = 0; i < N; i++) {
            int a;
            std::cin >> a;
        }
        std::cout << N << std::endl;
        return 0;
    }
    
    // Factorize K
    std::map<int, int> k_factors = factorize(K);
    
    // Read the sequence
    std::vector<int> seq(N);
    for (int i = 0; i < N; i++) {
        std::cin >> seq[i];
    }
    
    int count = 0;
    
    for (int a : seq) {
        bool valid = true;
        
        // Check each prime factor of K
        for (const auto& [p, exp_k] : k_factors) {
            // Count exponent of p in a
            int exp_a = 0;
            int temp = a;
            while (temp % p == 0) {
                exp_a++;
                temp /= p;
            }
            
            // Check if exp_a * M >= exp_k
            if (1LL * exp_a * M < exp_k) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            count++;
        }
    }
    
    std::cout << count << std::endl;
    
    return 0;
}
