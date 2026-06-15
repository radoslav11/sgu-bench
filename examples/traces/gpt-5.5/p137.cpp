#include <bits/stdc++.h>
using namespace std;

int egcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    int m = K % N;
    int q = K / N;

    int x, y;
    egcd(m, N, x, y);

    int inv = (x % N + N) % N;
    int r = (N - inv) % N;

    vector<int> ans(N, q);

    for (int j = 1; j <= m; j++) {
        int pos = (j * r) % N;
        ans[pos]++;
    }

    for (int i = 0; i < N; i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
