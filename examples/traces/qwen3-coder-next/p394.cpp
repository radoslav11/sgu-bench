#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Fenw {
    vector<int> tree;
    int n;
    Fenw(int size) : n(size), tree(size + 1, 0) {}
    void update(int idx, int delta) {
        while (idx <= n) {
            tree[idx] += delta;
            idx += idx & -idx;
        }
    }
    int prefix_query(int idx) {
        int sum = 0;
        while (idx > 0) {
            sum += tree[idx];
            idx -= idx & -idx;
        }
        return sum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N, K;
    cin >> N >> K;

    vector<ll> x(N), y(N), w(N);
    vector<ll> u_vec(N), v_vec(N);

    for (int i = 0; i < N; i++) {
        cin >> x[i] >> y[i] >> w[i];
        u_vec[i] = x[i] + y[i];
        v_vec[i] = x[i] - y[i];
    }

    vector<tuple<ll, int, ll, ll>> events;
    vector<ll> all_v;

    for (int i = 0; i < N; i++) {
        ll L = u_vec[i] - w[i];
        ll R = u_vec[i] + w[i];
        events.emplace_back(L, 1, v_vec[i] - w[i], v_vec[i] + w[i]);
        events.emplace_back(R + 1, -1, v_vec[i] - w[i], v_vec[i] + w[i]);

        all_v.push_back(v_vec[i] - w[i]);
        all_v.push_back(v_vec[i] + w[i] + 1);
    }

    for (int j = 0; j < N; j++) {
        events.emplace_back(u_vec[j], 0, v_vec[j], (ll)j);
        all_v.push_back(v_vec[j]);
    }

    sort(all_v.begin(), all_v.end());
    auto last = unique(all_v.begin(), all_v.end());
    all_v.erase(last, all_v.end());

    auto get_idx = [&](ll val) {
        return (int)(lower_bound(all_v.begin(), all_v.end(), val) - all_v.begin()) + 1;
    };

    sort(events.begin(), events.end(), [](const auto &a, const auto &b) {
        if (get<0>(a) != get<0>(b)) 
            return get<0>(a) < get<0>(b);
        auto rank = [](int t) {
            if (t == 1) return 0;
            if (t == 0) return 1;
            return 2;
        };
        return rank(get<1>(a)) < rank(get<1>(b));
    });

    Fenw fenw((int)all_v.size());
    vector<int> count_vec(N, 0);

    for (auto &event : events) {
        ll u = get<0>(event);
        int type = get<1>(event);
        ll L = get<2>(event);
        ll R = get<3>(event);

        if (type == 1 || type == -1) {
            int posL = get_idx(L);
            ll R1 = R + 1;
            int posR1 = get_idx(R1);

            if (type == 1) {
                fenw.update(posL, 1);
                fenw.update(posR1, -1);
            } else {
                fenw.update(posL, -1);
                fenw.update(posR1, 1);
            }
        } else {
            ll v_val = L;
            int pos = get_idx(v_val);
            int cnt = fenw.prefix_query(pos);
            int j = (int)get<3>(event);
            count_vec[j] = cnt;
        }
    }

    vector<int> closable;
    for (int j = 0; j < N; j++) {
        if (count_vec[j] - 1 >= K) {
            closable.push_back(j + 1);
        }
    }

    cout << (int)closable.size() << '\n';
    for (int i = 0; i < (int)closable.size(); i++) {
        if (i > 0) cout << ' ';
        cout << closable[i];
    }
    if (!closable.empty()) {
        cout << '\n';
    }

    return 0;
}
