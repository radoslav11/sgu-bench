#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> a(N);
    for (int i = 0; i < N; ++i) {
        cin >> a[i];
    }

    int ans = 0;

    for (int first = 0; first <= 1; ++first) {
        int prev = 0;
        int cur = first;
        bool ok = true;

        for (int i = 0; i < N - 1; ++i) {
            int nxt = a[i] - prev - cur;
            if (nxt < 0 || nxt > 1) {
                ok = false;
                break;
            }
            prev = cur;
            cur = nxt;
        }

        if (ok && a[N - 1] == prev + cur) {
            ++ans;
        }
    }

    cout << ans << '\n';

    return 0;
}
