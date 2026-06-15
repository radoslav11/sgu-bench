#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<bool> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return is_prime;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    // Generate primes up to N using Sieve of Eratosthenes
    vector<bool> is_prime = sieve(N);
    vector<int> primes;
    for (int i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    
    // We need to find pairs (A, B) such that A <= B, A + B <= N, and A+B is prime
    vector<pair<int, int>> result;
    
    // For each prime A
    for (int i = 0; i < primes.size(); ++i) {
        int A = primes[i];
        
        // For each prime B >= A such that A + B <= N
        for (int j = i; j < primes.size(); ++j) {
            int B = primes[j];
            int sum = A + B;
            
            if (sum > N) break;
            
            if (is_prime[sum]) {
                result.push_back({A, B});
            }
        }
    }
    
    // Output
    cout << result.size() << "\n";
    for (auto& p : result) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
