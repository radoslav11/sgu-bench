#include <bits/stdc++.h>
using namespace std;

const int MAX = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Precompute the Möbius function up to MAX
    vector<int> mu(MAX + 1, 1);
    vector<bool> is_composite(MAX + 1, false);
    vector<int> primes;
    mu[1] = 1;
    for (int i = 2; i <= MAX; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes.size() && i * primes[j] <= MAX; j++) {
            is_composite[i * primes[j]] = true;
            if (i % primes[j] == 0) {
                mu[i * primes[j]] = 0;
                break;
            } else {
                mu[i * primes[j]] = -mu[i];
            }
        }
    }

    int N, M;
    cin >> N >> M;

    long long A = M - 1;
    long long B = N - 1;

    // If there are no enemies
    if (A == 0 && B == 0) {
        cout << 0 << '\n';
        return 0;
    }

    long long ans = 0;
    if (M > 1) ans++;
    if (N > 1) ans++;

    int max_d = max(A, B);
    if (max_d > MAX) {
        max_d = MAX;
    }

    for (int d = 1; d <= max_d; d++) {
        ans += (long long)mu[d] * (A / d) * (B / d);
    }

    cout << ans << '\n';
    return 0;
}
