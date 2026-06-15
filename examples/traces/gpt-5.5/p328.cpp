#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string str;
    cin >> N >> str;

    vector<int> F(N + 1, 0), E(N + 1, 0), S(N + 1, 0), D(N + 1, 0);

    vector<int> seen(4 * N + 20, 0);
    int timer = 0;

    auto get_mex = [&](const vector<int>& vals) {
        ++timer;
        for (int x : vals) {
            if (x >= (int)seen.size()) seen.resize(x + 10, 0);
            seen[x] = timer;
        }
        int g = 0;
        while (g < (int)seen.size() && seen[g] == timer) ++g;
        return g;
    };

    for (int n = 1; n <= N; ++n) {
        vector<int> vals;

        // F[n]: segment with no colored endpoints
        vals.clear();
        for (int k = 1; k <= n; ++k) {
            vals.push_back(E[k - 1] ^ E[n - k]);
        }
        F[n] = get_mex(vals);

        // E[n]: segment with one colored endpoint
        vals.clear();
        for (int k = 1; k <= n; ++k) {
            // chosen color differs from the endpoint color
            vals.push_back(D[k - 1] ^ E[n - k]);

            // chosen color equals endpoint color, possible only if not adjacent
            if (k > 1) {
                vals.push_back(S[k - 1] ^ E[n - k]);
            }
        }
        E[n] = get_mex(vals);

        // S[n]: endpoints have the same color
        vals.clear();
        for (int k = 1; k <= n; ++k) {
            for (int c = 0; c <= 1; ++c) {
                // left endpoint color = 0, right endpoint color = 0
                if (k == 1 && c == 0) continue;
                if (k == n && c == 0) continue;

                int left = (c == 0 ? S[k - 1] : D[k - 1]);
                int right = (c == 0 ? S[n - k] : D[n - k]);

                vals.push_back(left ^ right);
            }
        }
        S[n] = get_mex(vals);

        // D[n]: endpoints have different colors
        vals.clear();
        for (int k = 1; k <= n; ++k) {
            for (int c = 0; c <= 1; ++c) {
                // left endpoint color = 0, right endpoint color = 1
                if (k == 1 && c == 0) continue;
                if (k == n && c == 1) continue;

                int left = (c == 0 ? S[k - 1] : D[k - 1]);
                int right = (c == 1 ? S[n - k] : D[n - k]);

                vals.push_back(left ^ right);
            }
        }
        D[n] = get_mex(vals);
    }

    int grundy = 0;

    for (int i = 0; i < N; ) {
        if (str[i] != '0') {
            ++i;
            continue;
        }

        int l = i;
        while (i < N && str[i] == '0') ++i;
        int r = i - 1;

        int len = r - l + 1;
        bool hasLeft = (l > 0);
        bool hasRight = (r + 1 < N);

        if (!hasLeft && !hasRight) {
            grundy ^= F[len];
        } else if (hasLeft && hasRight) {
            if (str[l - 1] == str[r + 1]) grundy ^= S[len];
            else grundy ^= D[len];
        } else {
            grundy ^= E[len];
        }
    }

    cout << (grundy ? "FIRST" : "SECOND") << '\n';

    return 0;
}
