#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

struct Fenw {
    int n;
    vector<ll> tree;
    Fenw(int size) {
        n = size;
        tree.assign(n+1, 0);
    }
    void update(int idx, ll val) {
        for (int i = idx+1; i <= n; i += i & -i) {
            tree[i] += val;
        }
    }
    ll query(int idx) {
        if (idx < 0) return 0;
        if (idx >= n) idx = n-1;
        ll res = 0;
        for (int i = idx+1; i; i -= i & -i) {
            res += tree[i];
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;
    vector<ll> W(N+1), L(N+1), P(N+1);
    for (int i=1; i<=N; i++) {
        cin >> W[i] >> L[i] >> P[i];
    }

    vector<ll> S(N+1, 0);
    for (int i=1; i<=N; i++) {
        S[i] = S[i-1] + W[i];
    }

    vector<ll> T(N+1);
    for (int i=1; i<=N; i++) {
        T[i] = S[i] - L[i];
    }

    vector<ll> all_vals;
    for (int i=1; i<=N; i++) {
        all_vals.push_back(T[i]);
    }
    for (int s=1; s<=N; s++) {
        all_vals.push_back(S[s-1]);
    }

    sort(all_vals.begin(), all_vals.end());
    auto last = unique(all_vals.begin(), all_vals.end());
    all_vals.erase(last, all_vals.end());
    int M = all_vals.size();

    Fenw fenw(M);

    vector<ll> F(N+1, 0);

    for (int s = N; s >= 1; s--) {
        int posT = lower_bound(all_vals.begin(), all_vals.end(), T[s]) - all_vals.begin();
        fenw.update(posT, P[s]);

        ll threshold = S[s-1];
        auto it = upper_bound(all_vals.begin(), all_vals.end(), threshold);
        int posQ = it - all_vals.begin() - 1;
        F[s] = fenw.query(posQ);
    }

    ll min_cost = (ll)1e18;
    int best_s = -1;
    for (int s=1; s<=N; s++) {
        if (F[s] < min_cost) {
            min_cost = F[s];
            best_s = s;
        }
    }

    vector<int> res;
    ll water = 0;
    for (int i = best_s; i <= N; i++) {
        water += W[i];
        if (water <= L[i]) {
            res.push_back(i);
        }
    }

    for (int idx : res) {
        cout << idx << '\n';
    }

    return 0;
}
