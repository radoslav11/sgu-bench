#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int SIZE = 1048576;
const int MAX_C = 1000000;

int c[MAXN];
int dp[MAXN];
int parent_node[MAXN];
pair<int, int> tree_node[2 * SIZE];

pair<int, int> better(pair<int, int> a, pair<int, int> b) {
    if (a.first > b.first) return a;
    if (b.first > a.first) return b;
    return a;
}

void update(int pos, pair<int, int> val) {
    pos += SIZE;
    tree_node[pos] = better(tree_node[pos], val);
    for (pos /= 2; pos > 0; pos /= 2) {
        tree_node[pos] = better(tree_node[2 * pos], tree_node[2 * pos + 1]);
    }
}

pair<int, int> query(int l, int r) {
    if (l > r) return {0, 0};
    l += SIZE;
    r += SIZE;
    pair<int, int> res = {0, 0};
    while (l <= r) {
        if (l % 2 == 1) {
            res = better(res, tree_node[l]);
            l++;
        }
        if (r % 2 == 0) {
            res = better(res, tree_node[r]);
            r--;
        }
        l /= 2;
        r /= 2;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    for (int i = 1; i <= n; ++i) {
        cin >> c[i];
    }

    int max_len = 0;
    int best_idx = -1;

    for (int i = 1; i <= n; ++i) {
        pair<int, int> res1 = query(0, c[i] - 2);
        pair<int, int> res2 = query(c[i], c[i]);
        pair<int, int> res3 = query(c[i] + 2, MAX_C);

        pair<int, int> best1 = better(res1, res2);
        pair<int, int> best_overall = better(best1, res3);

        dp[i] = best_overall.first + 1;
        parent_node[i] = best_overall.second;

        if (dp[i] > max_len) {
            max_len = dp[i];
            best_idx = i;
        }

        update(c[i], {dp[i], i});
    }

    vector<int> ans;
    int curr = best_idx;
    while (curr != 0) {
        ans.push_back(c[curr]);
        curr = parent_node[curr];
    }
    reverse(ans.begin(), ans.end());

    cout << n - ans.size() << "\n";
    for (int i = 0; i < (int)ans.size(); ++i) {
        cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
