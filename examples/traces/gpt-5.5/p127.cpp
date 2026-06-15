#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K, N;
    cin >> K >> N;

    vector<int> cnt(10, 0);

    for (int i = 0; i < N; ++i) {
        string phone;
        cin >> phone;
        cnt[phone[0] - '0']++;
    }

    int pages = 2;
    for (int d = 0; d <= 9; ++d) {
        if (cnt[d] > 0) {
            pages += (cnt[d] + K - 1) / K;
        }
    }

    cout << pages << '\n';
    return 0;
}
