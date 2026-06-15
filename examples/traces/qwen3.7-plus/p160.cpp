#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<int> a(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> a[i];
    }

    vector<bool> dp(M, false);
    vector<int> parent(M, -1);
    vector<int> last_lever(M, -1);

    dp[1] = true;
    vector<int> active;
    active.push_back(1);

    for (int i = 1; i <= N; ++i) {
        int v = a[i] % M;
        int sz = active.size();
        for (int k = 0; k < sz; ++k) {
            int j = active[k];
            int nxt = (j * v) % M;
            if (!dp[nxt]) {
                dp[nxt] = true;
                parent[nxt] = j;
                last_lever[nxt] = i;
                active.push_back(nxt);
            }
        }
    }

    int max_score = 1;
    for (int j = 1; j < M; ++j) {
        if (dp[j] && j > max_score) {
            max_score = j;
        }
    }

    vector<int> ans;
    int curr = max_score;
    while (parent[curr] != -1) {
        ans.push_back(last_lever[curr]);
        curr = parent[curr];
    }

    reverse(ans.begin(), ans.end());

    cout << max_score << "\n";
    if (!ans.empty()) {
        for (int i = 0; i < (int)ans.size(); ++i) {
            cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
