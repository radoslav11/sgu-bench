#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 21) {
        cout << 1 << '\n';
        return 0;
    }

    int h = max(2, (n - 12 + 13) / 14); // ceil((n - 12) / 14), at least 2
    int rest = n - 14 * h;

    if (rest >= 2 && rest <= 12)
        cout << h << '\n';
    else
        cout << -1 << '\n';

    return 0;
}
