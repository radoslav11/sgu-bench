#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, q;
    cin >> N >> q;

    long long ans = 0;

    while (N > 1) {
        long long k = N / 2;

        if (q > k) {
            q = N - q + 1;
            N = N - k;
        } else {
            ans += N - k;
            q = k - q + 1;
            N = k;
        }
    }

    cout << ans + 1 << '\n';

    return 0;
}
