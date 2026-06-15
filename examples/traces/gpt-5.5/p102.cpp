#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int result = n;
    int x = n;

    for (int p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            result -= result / p;
        }
    }

    if (x > 1) {
        result -= result / x;
    }

    cout << result << '\n';
    return 0;
}
