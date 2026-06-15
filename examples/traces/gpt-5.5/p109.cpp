#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    auto id = [&](int r, int c) {
        return (r - 1) * N + c;
    };

    int K1 = N;
    int K2;
    if (N % 2 == 0) K2 = N + 1;
    else K2 = N + 2;

    int S1 = K1;
    int S2 = K1 + K2;

    // Choose a final picture whose parity is opposite to S1 and equal to S2.
    int tr = 1, tc;
    if (S2 % 2 == 0) tc = 1; // distance 0, even
    else tc = 2;             // distance 1, odd

    vector<vector<bool>> removed(N + 1, vector<bool>(N + 1, false));

    vector<int> first;
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= N; ++c) {
            if (r == tr && c == tc) continue;
            int d = (r - 1) + (c - 1);
            if (d > S1 || (d % 2) != (S1 % 2)) {
                first.push_back(id(r, c));
                removed[r][c] = true;
            }
        }
    }

    vector<int> second;
    for (int r = 1; r <= N; ++r) {
        for (int c = 1; c <= N; ++c) {
            if (r == tr && c == tc) continue;
            if (removed[r][c]) continue;
            int d = (r - 1) + (c - 1);
            if (d <= S2 && (d % 2) != (S2 % 2)) {
                second.push_back(id(r, c));
                removed[r][c] = true;
            }
        }
    }

    cout << K1;
    for (int x : first) cout << ' ' << x;
    cout << '\n';

    cout << K2;
    for (int x : second) cout << ' ' << x;
    cout << '\n';

    return 0;
}
