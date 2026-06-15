#include <bits/stdc++.h>
using namespace std;

const int MAX_COLOR = 1000010;

struct SegTree {
    vector<int> val;
    vector<int> col;
    int n;

    SegTree(int size) {
        n = size;
        val.assign(4 * n, 0);
        col.assign(4 * n, -1);
    }

    void update(int idx, int v, int node, int l, int r) {
        if (l == r) {
            if (v > val[node]) {
                val[node] = v;
                col[node] = idx;
            }
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) {
            update(idx, v, 2 * node + 1, l, mid);
        } else {
            update(idx, v, 2 * node + 2, mid + 1, r);
        }
        if (val[2 * node + 1] >= val[2 * node + 2]) {
            val[node] = val[2 * node + 1];
            col[node] = col[2 * node + 1];
        } else {
            val[node] = val[2 * node + 2];
            col[node] = col[2 * node + 2];
        }
    }

    void update(int idx, int v) {
        update(idx, v, 0, 0, n - 1);
    }

    pair<int, int> query(int l_query, int r_query, int node, int l, int r) {
        if (l_query > r_query) {
            return {0, -1};
        }
        if (l_query <= l && r <= r_query) {
            return {val[node], col[node]};
        }
        int mid = (l + r) / 2;
        pair<int, int> left_res = {0, -1};
        pair<int, int> right_res = {0, -1};
        if (l_query <= mid) {
            left_res = query(l_query, r_query, 2 * node + 1, l, mid);
        }
        if (r_query > mid) {
            right_res = query(l_query, r_query, 2 * node + 2, mid + 1, r);
        }
        if (left_res.first >= right_res.first) {
            return left_res;
        } else {
            return right_res;
        }
    }

    pair<int, int> query(int l_query, int r_query) {
        if (l_query > r_query) {
            return {0, -1};
        }
        return query(l_query, r_query, 0, 0, n - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> c(n);
    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }

    SegTree seg(MAX_COLOR);
    vector<int> best_val(MAX_COLOR, 0);
    vector<int> best_idx(MAX_COLOR, -1);
    vector<int> dp(n, 0);
    vector<int> prev_index(n, -1);

    for (int i = 0; i < n; i++) {
        int x = c[i];
        int candidate_val = 0;
        int candidate_col = -1;

        if (x >= 2) {
            auto res = seg.query(0, x - 2);
            if (res.first > candidate_val) {
                candidate_val = res.first;
                candidate_col = res.second;
            }
        }

        if (x + 2 < MAX_COLOR) {
            auto res = seg.query(x + 2, MAX_COLOR - 1);
            if (res.first > candidate_val) {
                candidate_val = res.first;
                candidate_col = res.second;
            }
        }

        if (best_val[x] > candidate_val) {
            candidate_val = best_val[x];
            candidate_col = x;
        }

        dp[i] = candidate_val + 1;
        if (candidate_val > 0) {
            prev_index[i] = best_idx[candidate_col];
        }

        if (dp[i] > best_val[x]) {
            best_val[x] = dp[i];
            best_idx[x] = i;
            seg.update(x, dp[i]);
        }
    }

    int max_dp = 0;
    int end_index = -1;
    for (int i = 0; i < n; i++) {
        if (dp[i] > max_dp) {
            max_dp = dp[i];
            end_index = i;
        }
    }

    vector<int> res_vec;
    int cur = end_index;
    while (cur != -1) {
        res_vec.push_back(c[cur]);
        cur = prev_index[cur];
    }
    reverse(res_vec.begin(), res_vec.end());

    cout << n - max_dp << "\n";
    for (int i = 0; i < res_vec.size(); i++) {
        if (i > 0) cout << " ";
        cout << res_vec[i];
    }
    cout << "\n";

    return 0;
}
