#include <bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    while (b) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long phi(long long n) {
    long long result = n;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

long long power(long long base, long long exp) {
    long long result = 1;
    for (long long i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    cin >> n;
    
    long long sum = 0;
    
    // Find all divisors of n
    for (long long d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            // d is a divisor
            sum += phi(d) * power(2, n / d);
            
            // n/d is also a divisor (if different from d)
            if (d != n / d) {
                long long d2 = n / d;
                sum += phi(d2) * power(2, n / d2);
            }
        }
    }
    
    long long result = sum / n;
    cout << result << endl;
    
    return 0;
}
