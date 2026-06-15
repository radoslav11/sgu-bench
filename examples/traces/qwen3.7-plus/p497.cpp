#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    if (!(cin >> n)) return 0;
    
    // Precompute the number of integer partitions up to 40.
    // 2^39 > 10^12, so the maximum possible exponent for any prime factor is 39.
    vector<long long> P(45, 0);
    P[0] = 1;
    for (int i = 1; i <= 40; ++i) {
        for (int j = i; j <= 40; ++j) {
            P[j] += P[j - i];
        }
    }
    
    long long ans = 1;
    
    // Check for factor of 2
    if (n % 2 == 0) {
        int count = 0;
        while (n % 2 == 0) {
            count++;
            n /= 2;
        }
        ans *= P[count];
    }
    
    // Check for odd factors up to sqrt(n)
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                count++;
                n /= i;
            }
            ans *= P[count];
        }
    }
    
    // If n is still greater than 1, it must be a prime number itself.
    // Its exponent is exactly 1, and P[1] = 1, so multiplying by P[1] keeps 'ans' unchanged.
    if (n > 1) {
        ans *= P[1];
    }
    
    cout << ans << "\n";
    
    return 0;
}
