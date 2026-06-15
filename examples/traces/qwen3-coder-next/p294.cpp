#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

const int MAX_N = 200000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    // Precompute Euler's totient function for 1 to MAX_N
    vector<int> phi(MAX_N + 1);
    for (int i = 1; i <= MAX_N; i++) {
        phi[i] = i;
    }
    for (int i = 2; i <= MAX_N; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= MAX_N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    int n;
    cin >> n;

    // Find all divisors of n
    vector<int> divisors;
    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }

    cpp_int total = 0;
    for (int d : divisors) {
        int exponent = n / d;
        // Compute 2^exponent using bit shift
        cpp_int term = cpp_int(1) << exponent;
        term *= phi[d];
        total += term;
    }

    total /= n;
    cout << total << '\n';

    return 0;
}
