#include <bits/stdc++.h>
using namespace std;

bool isPrimeNumber(long long x, const vector<int>& primes) {
    if (x < 2) return false;
    for (int p : primes) {
        if (1LL * p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int LIMIT = 31623;
    vector<bool> isPrime(LIMIT + 1, true);
    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) {
                isPrime[j] = false;
            }
        }
    }

    vector<int> primes;
    for (int i = 2; i <= LIMIT; ++i) {
        if (isPrime[i]) primes.push_back(i);
    }

    int N;
    cin >> N;

    while (N--) {
        long long x;
        cin >> x;

        bool nearlyPrime = false;

        for (int p : primes) {
            if (1LL * p * p > x) break;

            if (x % p == 0) {
                long long q = x / p;
                if (isPrimeNumber(q, primes)) {
                    nearlyPrime = true;
                }
                break;
            }
        }

        cout << (nearlyPrime ? "Yes" : "No") << '\n';
    }

    return 0;
}
