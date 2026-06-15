#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (n % i == 0 && m % j == 0) {
                if (__gcd(n / i, m / j) == 1) {
                    ans += 1LL * i * j;
                }
            }
        }
    }
    cout << ans << '\n';
}
