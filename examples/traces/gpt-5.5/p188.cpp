#include <bits/stdc++.h>
using namespace std;

long long floordiv(long long a, long long b) {
    // b > 0
    if (a >= 0) return a / b;
    return - ((-a + b - 1) / b);
}

long long ceildiv(long long a, long long b) {
    // b > 0
    return -floordiv(-a, b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long T;
    cin >> N >> T;

    vector<long long> L(N), V(N);
    for (int i = 0; i < N; ++i) cin >> L[i];
    for (int i = 0; i < N; ++i) cin >> V[i];

    vector<long long> ans(N, 0);

    const long long P = 1000;

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (V[i] * V[j] >= 0) continue;

            long long a = V[i] - V[j];
            long long b = L[j] - L[i];

            long long low = min(0LL, a * T);
            long long high = max(0LL, a * T);

            long long kLow = ceildiv(low - b, P);
            long long kHigh = floordiv(high - b, P);

            if (kLow <= kHigh) {
                long long cnt = kHigh - kLow + 1;
                ans[i] += cnt;
                ans[j] += cnt;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
