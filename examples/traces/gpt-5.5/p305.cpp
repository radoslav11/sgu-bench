#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll ceil_div(ll a, ll b) { // b > 0
    if (a >= 0) return (a + b - 1) / b;
    return a / b; // C++ truncation toward zero is ceil for negative a
}

ll floor_div(ll a, ll b) { // b > 0
    if (a >= 0) return a / b;
    return - ((-a + b - 1) / b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    ll M;
    cin >> N >> M;

    vector<ll> A(N), B(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i] >> B[i];
    }

    unordered_map<ll, int> id;
    id.reserve(N * N * 2);

    vector<ll> values;
    vector<vector<int>> g(N);

    auto get_id = [&](ll x) -> int {
        auto it = id.find(x);
        if (it != id.end()) return it->second;
        int new_id = (int)values.size();
        id[x] = new_id;
        values.push_back(x);
        return new_id;
    };

    for (int i = 0; i < N; i++) {
        vector<ll> cur;

        if (A[i] == 0) {
            if (1 <= B[i] && B[i] <= M) {
                cur.push_back(B[i]);
            }
        } else if (A[i] > 0) {
            ll a = A[i], b = B[i];
            ll k = max(0LL, ceil_div(1 - b, a));

            for (int cnt = 0; cnt < N; cnt++, k++) {
                ll x = b + a * k;
                if (x > M) break;
                if (x >= 1) cur.push_back(x);
            }
        } else {
            ll d = -A[i], b = B[i];

            ll low = 0;
            if (b > M) low = ceil_div(b - M, d);

            ll high = floor_div(b - 1, d);

            for (ll k = high; k >= low && (int)cur.size() < N; k--) {
                ll x = b - d * k;
                if (1 <= x && x <= M) cur.push_back(x);
                if (k == LLONG_MIN) break;
            }
        }

        sort(cur.begin(), cur.end());
        cur.erase(unique(cur.begin(), cur.end()), cur.end());

        for (ll x : cur) {
            g[i].push_back(get_id(x));
        }
    }

    int R = (int)values.size();
    vector<int> pairU(N, -1), pairV(R, -1), dist(N);

    auto bfs = [&]() {
        queue<int> q;
        bool found = false;

        for (int u = 0; u < N; u++) {
            if (pairU[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                int u2 = pairV[v];
                if (u2 == -1) {
                    found = true;
                } else if (dist[u2] == -1) {
                    dist[u2] = dist[u] + 1;
                    q.push(u2);
                }
            }
        }

        return found;
    };

    function<bool(int)> dfs = [&](int u) {
        for (int v : g[u]) {
            int u2 = pairV[v];
            if (u2 == -1 || (dist[u2] == dist[u] + 1 && dfs(u2))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    };

    while (bfs()) {
        for (int u = 0; u < N; u++) {
            if (pairU[u] == -1) {
                dfs(u);
            }
        }
    }

    vector<ll> ans(N, -1);
    set<ll> used;

    for (int i = 0; i < N; i++) {
        if (pairU[i] != -1) {
            ans[i] = values[pairU[i]];
            used.insert(ans[i]);
        }
    }

    ll x = 1;
    for (int i = 0; i < N; i++) {
        if (ans[i] == -1) {
            while (used.count(x)) x++;
            ans[i] = x;
            used.insert(x);
        }
    }

    for (int i = 0; i < N; i++) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
