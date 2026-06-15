#include <bits/stdc++.h>
using namespace std;

long long trailingZeros(long long n) {
    long long res = 0;
    while (n > 0) {
        n /= 5;
        res += n;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long Q;
    cin >> Q;

    if (Q == 0) {
        cout << 1 << '\n';
        return 0;
    }

    long long left = 1, right = 5 * (Q + 1);

    while (left < right) {
        long long mid = (left + right) / 2;
        if (trailingZeros(mid) >= Q)
            right = mid;
        else
            left = mid + 1;
    }

    if (trailingZeros(left) == Q)
        cout << left << '\n';
    else
        cout << "No solution\n";

    return 0;
}
