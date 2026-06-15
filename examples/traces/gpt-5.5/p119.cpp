#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, A0, B0;
    if (!(cin >> N >> A0 >> B0)) return 0;

    int d = gcd(N, gcd(A0, B0));
    int cnt = N / d;

    vector<pair<int, int>> ans;
    ans.reserve(cnt);

    for (int k = 0; k < cnt; ++k) {
        int A = (int)((1LL * k * A0) % N);
        int B = (int)((1LL * k * B0) % N);
        ans.push_back({A, B});
    }

    sort(ans.begin(), ans.end());

    cout << ans.size() << '\n';
    for (auto [A, B] : ans) {
        cout << A << ' ' << B << '\n';
    }

    return 0;
}
