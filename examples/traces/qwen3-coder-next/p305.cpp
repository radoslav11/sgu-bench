#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <algorithm>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    ll N, M;
    cin >> N >> M;

    vector<pair<ll, ll>> progs;
    for (int i = 0; i < N; i++) {
        ll a, b;
        cin >> a >> b;
        progs.push_back({a, b});
    }

    vector<vector<ll>> all_candidates(N);

    for (int i = 0; i < N; i++) {
        ll A = progs[i].first;
        ll B = progs[i].second;
        vector<ll> cand;

        if (A == 0) {
            if (B >= 1 && B <= M) {
                cand.push_back(B);
            } else {
                cand.push_back(M + 1);
            }
        } else if (A > 0) {
            ll x = B;
            if (x < 1) {
                ll diff = 1 - B;
                ll k = (diff + A - 1) / A;
                x = B + k * A;
            }
            for (int j = 0; j <= N; j++) {
                if (x > M) break;
                if (x >= 1) {
                    cand.push_back(x);
                }
                x += A;
            }
        } else {
            ll A_abs = -A;
            if (B >= 1) {
                ll k = (B - 1) / A_abs;
                ll x = B - k * A_abs;
                for (int j = 0; j <= N; j++) {
                    if (x > M) break;
                    if (x >= 1) {
                        cand.push_back(x);
                    }
                    x += A_abs;
                }
            } else {
                cand.push_back(M + 1);
            }
        }

        if (cand.empty()) {
            cand.push_back(M + 1);
        }
        all_candidates[i] = cand;
    }

    set<ll> all_set;
    for (int i = 0; i < N; i++) {
        for (ll x : all_candidates[i]) {
            if (x >= 1 && x <= M) {
                all_set.insert(x);
            }
        }
    }

    map<ll, int> cand_to_index;
    int idx = 0;
    for (ll x : all_set) {
        cand_to_index[x] = idx++;
    }
    int right_size = all_set.size();

    vector<vector<int>> graph(N);
    for (int i = 0; i < N; i++) {
        for (ll x : all_candidates[i]) {
            if (x >= 1 && x <= M && cand_to_index.find(x) != cand_to_index.end()) {
                int j = cand_to_index[x];
                graph[i].push_back(j);
            }
        }
    }

    vector<int> match_to_right(right_size, -1);
    vector<int> match_to_left(N, -1);

    function<bool(int, vector<bool>&)> dfs = [&](int i, vector<bool>& seen) {
        for (int j : graph[i]) {
            if (seen[j]) continue;
            seen[j] = true;
            if (match_to_right[j] == -1 || dfs(match_to_right[j], seen)) {
                match_to_right[j] = i;
                return true;
            }
        }
        return false;
    };

    for (int i = 0; i < N; i++) {
        vector<bool> seen(right_size, false);
        if (dfs(i, seen)) {
            for (int j = 0; j < right_size; j++) {
                if (match_to_right[j] == i) {
                    match_to_left[i] = j;
                    break;
                }
            }
        }
    }

    vector<ll> ans(N);
    for (int i = 0; i < N; i++) {
        int j = match_to_left[i];
        if (j == -1) {
            ans[i] = all_candidates[i][0];
        } else {
            ans[i] = all_set[i] ? *next(all_set.begin(), j) : M + 1;
        }
    }

    for (int i = 0; i < N; i++) {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << endl;

    return 0;
}
