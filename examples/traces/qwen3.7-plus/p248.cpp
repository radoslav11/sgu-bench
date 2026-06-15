#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

long long modInverse(long long a, long long m) {
    long long x, y;
    extGCD(a, m, x, y);
    return (x % m + m) % m;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    if (!(cin >> N)) return 0;
    
    vector<long long> c(N);
    for (int i = 0; i < N; ++i) {
        cin >> c[i];
    }
    
    long long V;
    cin >> V;

    long long ans = -1;

    if (N == 1) {
        if (V % c[0] == 0) {
            ans = V / c[0];
        }
    } else if (N == 2) {
        for (long long x1 = 0; x1 <= V / c[0]; ++x1) {
            long long V_prime = V - c[0] * x1;
            if (V_prime % c[1] == 0) {
                long long current_sum = x1 + V_prime / c[1];
                if (ans == -1 || current_sum < ans) {
                    ans = current_sum;
                }
            }
        }
    } else if (N == 3) {
        long long g = std::gcd(c[1], c[2]);
        long long c2_prime = c[1] / g;
        long long c3_prime = c[2] / g;
        long long inv = -1;
        if (c3_prime > 1) {
            inv = modInverse(c2_prime, c3_prime);
        }
        
        for (long long x1 = 0; x1 <= V / c[0]; ++x1) {
            long long V_prime = V - c[0] * x1;
            if (V_prime % g != 0) continue;
            
            long long V_g = V_prime / g;
            long long max_x2 = V_prime / c[1];
            long long x2_min, x2_max;
            
            if (c3_prime == 1) {
                x2_min = 0;
                x2_max = max_x2;
            } else {
                x2_min = (V_g % c3_prime) * inv % c3_prime;
                if (x2_min > max_x2) continue;
                long long k_max = (max_x2 - x2_min) / c3_prime;
                x2_max = x2_min + k_max * c3_prime;
            }
            
            long long x3_min = (V_prime - c[1] * x2_min) / c[2];
            long long sum_min = x1 + x2_min + x3_min;
            if (ans == -1 || sum_min < ans) {
                ans = sum_min;
            }
            
            if (x2_max != x2_min) {
                long long x3_max = (V_prime - c[1] * x2_max) / c[2];
                long long sum_max = x1 + x2_max + x3_max;
                if (ans == -1 || sum_max < ans) {
                    ans = sum_max;
                }
            }
        }
    }
    
    cout << ans << "\n";
    return 0;
}
