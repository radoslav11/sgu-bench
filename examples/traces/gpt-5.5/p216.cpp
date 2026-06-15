#include <bits/stdc++.h>
using namespace std;

int N, B;
vector<vector<int>> g;
vector<int> province, cap, st;
int K = 0;

void dfs(int v, int p) {
    int start = (int)st.size();

    for (int to : g[v]) {
        if (to == p) continue;

        dfs(to, v);

        if ((int)st.size() - start >= B) {
            ++K;
            cap.push_back(v);

            for (int i = start; i < (int)st.size(); ++i) {
                province[st[i]] = K;
            }

            st.resize(start);
        }
    }

    st.push_back(v);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> B;

    g.assign(N + 1, {});
    province.assign(N + 1, 0);
    cap.reserve(N + 1);
    cap.push_back(0);

    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dfs(1, 0);

    if (K == 0) {
        cout << 1 << '\n';
        for (int i = 1; i <= N; ++i) {
            if (i > 1) cout << ' ';
            cout << 1;
        }
        cout << '\n';
        cout << 1 << '\n';
        return 0;
    }

    for (int v : st) {
        province[v] = K;
    }

    cout << K << '\n';

    for (int i = 1; i <= N; ++i) {
        if (i > 1) cout << ' ';
        cout << province[i];
    }
    cout << '\n';

    for (int i = 1; i <= K; ++i) {
        if (i > 1) cout << ' ';
        cout << cap[i];
    }
    cout << '\n';

    return 0;
}
