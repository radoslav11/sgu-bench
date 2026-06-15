#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    unsigned long long basis[64] = {};

    for (int i = 0; i < N; ++i) {
        unsigned long long x;
        cin >> x;

        for (int b = 63; b >= 0; --b) {
            if (((x >> b) & 1ULL) == 0) continue;

            if (!basis[b]) {
                basis[b] = x;
                break;
            }

            x ^= basis[b];
        }
    }

    unsigned long long ans = 0;
    for (int b = 63; b >= 0; --b) {
        ans = max(ans, ans ^ basis[b]);
    }

    cout << ans << '\n';

    return 0;
}
