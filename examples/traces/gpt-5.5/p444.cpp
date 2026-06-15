#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, y;
    cin >> x >> y;

    vector<int> seen(y, -1);

    int rem = x % y;
    int pos = 0;

    while (rem != 0 && seen[rem] == -1) {
        seen[rem] = pos;
        rem = (int)((long long)rem * 10 % y);
        pos++;
    }

    if (rem == 0) {
        cout << pos << ' ' << 0 << '\n';
    } else {
        cout << seen[rem] << ' ' << pos - seen[rem] << '\n';
    }

    return 0;
}
