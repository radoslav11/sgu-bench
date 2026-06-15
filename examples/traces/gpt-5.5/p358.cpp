#include <bits/stdc++.h>
using namespace std;

int median3(int a, int b, int c) {
    if (a > b) swap(a, b);
    if (b > c) swap(b, c);
    if (a > b) swap(a, b);
    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m[3];
    for (int i = 0; i < 3; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        m[i] = median3(a, b, c);
    }

    cout << median3(m[0], m[1], m[2]) << '\n';
    return 0;
}
