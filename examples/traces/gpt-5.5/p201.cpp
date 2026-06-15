#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string alphabet;
    cin >> alphabet;
    int A = (int)alphabet.size();

    int K;
    cin >> K;

    int S, L;
    cin >> S >> L;
    --S;

    vector<int> terminal(K, 0);
    for (int i = 0; i < L; ++i) {
        int x;
        cin >> x;
        terminal[x - 1] = 1;
    }

    vector<vector<int>> phi(K, vector<int>(A));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < A; ++j) {
            cin >> phi[i][j];
            --phi[i][j];
        }
    }

    vector<vector<int>> chi(K, vector<int>(A));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < A; ++j) {
            cin >> chi[i][j];
        }
    }

    int N;
    cin >> N;

    vector<vector<int>> go(A, vector<int>(K, -1));

    for (int c = 0; c < A; ++c) {
        vector<int> color(K, 0);

        function<int(int)> dfs = [&](int u) -> int {
            if (color[u] == 2) return go[c][u];
            if (color[u] == 1) return -1;

            color[u] = 1;

            if (chi[u][c] == 0) {
                go[c][u] = phi[u][c];
            } else {
                go[c][u] = dfs(phi[u][c]);
            }

            color[u] = 2;
            return go[c][u];
        };

        for (int u = 0; u < K; ++u) {
            if (color[u] == 0) dfs(u);
        }
    }

    vector<cpp_int> dp(K), ndp(K);
    dp[S] = 1;

    for (int len = 0; len < N; ++len) {
        fill(ndp.begin(), ndp.end(), 0);

        for (int u = 0; u < K; ++u) {
            if (dp[u] == 0) continue;

            for (int c = 0; c < A; ++c) {
                int v = go[c][u];
                if (v != -1) {
                    ndp[v] += dp[u];
                }
            }
        }

        dp.swap(ndp);
    }

    cpp_int answer = 0;
    for (int u = 0; u < K; ++u) {
        if (terminal[u]) answer += dp[u];
    }

    cout << answer << '\n';

    return 0;
}
