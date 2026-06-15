#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, m;
    cin >> t >> m;

    vector<int> primes;
    for (int x = 2; (int)primes.size() < t; ++x) {
        bool ok = true;
        for (int p : primes) {
            if (p * p > x) break;
            if (x % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) primes.push_back(x);
    }

    vector<bitset<100>> basis(100);
    int rank = 0;

    for (int i = 0; i < m; ++i) {
        int b;
        cin >> b;

        bitset<100> v;
        for (int j = 0; j < t; ++j) {
            int cnt = 0;
            while (b % primes[j] == 0) {
                b /= primes[j];
                cnt ^= 1;
            }
            if (cnt) v.set(j);
        }

        for (int j = t - 1; j >= 0; --j) {
            if (!v[j]) continue;
            if (basis[j].none()) {
                basis[j] = v;
                ++rank;
                break;
            } else {
                v ^= basis[j];
            }
        }
    }

    cpp_int answer = (cpp_int(1) << (m - rank)) - 1;
    cout << answer << '\n';

    return 0;
}
