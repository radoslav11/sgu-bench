#include <bits/stdc++.h>
using namespace std;

long long comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    long long res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    if (k > n) {
        cout << 0 << '\n';
        return 0;
    }

    long long ans = comb(n, k) * comb(n, k);

    for (int i = 1; i <= k; ++i) {
        ans *= i;
    }

    cout << ans << '\n';

    return 0;
}
