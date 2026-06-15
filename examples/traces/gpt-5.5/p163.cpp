#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;

        int val = 1;
        for (int j = 0; j < p; ++j) val *= x;

        if (val > 0) ans += val;
    }

    cout << ans << '\n';
    return 0;
}
