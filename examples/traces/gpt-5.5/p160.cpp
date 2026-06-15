#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> a(N + 1);
    for (int i = 1; i <= N; ++i) cin >> a[i];

    vector<char> dp(M, 0);
    vector<int> prevResidue(M, -1), prevIndex(M, -1);

    dp[1 % M] = 1;

    for (int i = 1; i <= N; ++i) {
        vector<char> old = dp;

        for (int r = 0; r < M; ++r) {
            if (!old[r]) continue;

            int nr = (r * a[i]) % M;
            if (!dp[nr]) {
                dp[nr] = 1;
                prevResidue[nr] = r;
                prevIndex[nr] = i;
            }
        }
    }

    int best = 0;
    for (int r = M - 1; r >= 0; --r) {
        if (dp[r]) {
            best = r;
            break;
        }
    }

    vector<int> answer;
    int cur = best;

    while (!(cur == 1 && prevIndex[cur] == -1)) {
        answer.push_back(prevIndex[cur]);
        cur = prevResidue[cur];
    }

    reverse(answer.begin(), answer.end());

    cout << best << '\n';
    for (size_t i = 0; i < answer.size(); ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
