#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> cntX;
    vector<int> cntY;
    vector<int> sz;

    DSU(int n, int m) {
        parent.resize(n + m + 1);
        iota(parent.begin(), parent.end(), 0);
        cntX.assign(n + m + 1, 0);
        cntY.assign(n + m + 1, 0);
        sz.assign(n + m + 1, 1);
        for (int i = 1; i <= n; ++i) {
            cntX[i] = 1;
        }
        for (int i = 1; i <= m; ++i) {
            cntY[n + i] = 1;
        }
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) {
                swap(root_i, root_j);
            }
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            cntX[root_i] += cntX[root_j];
            cntY[root_i] += cntY[root_j];
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (!(cin >> K)) return 0;

    if (K == 0) {
        cout << 0 << "\n";
        return 0;
    }

    vector<int> X_vals(K), Y_vals(K);
    for (int i = 0; i < K; ++i) {
        cin >> X_vals[i] >> Y_vals[i];
    }

    vector<pair<int, int>> sorted_X(K);
    for (int i = 0; i < K; ++i) sorted_X[i] = {X_vals[i], i};
    sort(sorted_X.begin(), sorted_X.end());
    vector<int> x_id(K);
    int n = 0;
    for (int i = 0; i < K; ++i) {
        if (i == 0 || sorted_X[i].first != sorted_X[i-1].first) {
            n++;
        }
        x_id[sorted_X[i].second] = n;
    }

    vector<pair<int, int>> sorted_Y(K);
    for (int i = 0; i < K; ++i) sorted_Y[i] = {Y_vals[i], i};
    sort(sorted_Y.begin(), sorted_Y.end());
    vector<int> y_id(K);
    int m = 0;
    for (int i = 0; i < K; ++i) {
        if (i == 0 || sorted_Y[i].first != sorted_Y[i-1].first) {
            m++;
        }
        y_id[sorted_Y[i].second] = m;
    }

    DSU dsu(n, m);

    for (int i = 0; i < K; ++i) {
        dsu.unite(x_id[i], n + y_id[i]);
    }

    long long ans = 0;
    for (int i = 1; i <= n + m; ++i) {
        if (dsu.parent[i] == i) {
            ans += 1LL * dsu.cntX[i] * dsu.cntY[i];
        }
    }

    cout << ans << "\n";
    return 0;
}
