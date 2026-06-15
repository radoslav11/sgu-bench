#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    int M = 0, t = N;
    while (t > 0) {
        ++M;
        t >>= 1;
    }

    vector<int> lp(N + 1, 0), primes;
    vector<unsigned char> omega(N + 1, 0);

    for (int i = 2; i <= N; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
            omega[i] = 1;
        }

        for (int p : primes) {
            long long v = 1LL * i * p;
            if (v > N) break;

            lp[(int)v] = p;
            omega[(int)v] = omega[i] + 1;

            if (p == lp[i]) break;
        }
    }

    cout << M << '\n';
    for (int i = 1; i <= N; ++i) {
        if (i > 1) cout << ' ';
        cout << int(omega[i]) + 1;
    }
    cout << '\n';

    return 0;
}
