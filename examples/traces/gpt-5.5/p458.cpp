#include <bits/stdc++.h>
using namespace std;

struct Node {
    int len;
    int idx;
};

Node better(Node a, Node b) {
    if (a.len != b.len) return (a.len > b.len) ? a : b;
    return (a.idx > b.idx) ? a : b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    const int MAXC = 1000000;
    int SZ = 1;
    while (SZ <= MAXC) SZ <<= 1;

    vector<Node> seg(2 * SZ, {0, -1});
    vector<int> c(n), dp(n), parent(n, -1);

    auto query = [&](int l, int r) {
        if (l > r) return Node{0, -1};
        l += SZ;
        r += SZ;
        Node res{0, -1};
        while (l <= r) {
            if (l & 1) res = better(res, seg[l++]);
            if (!(r & 1)) res = better(res, seg[r--]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    };

    auto update = [&](int pos, Node val) {
        pos += SZ;
        if (seg[pos].len >= val.len) return;
        seg[pos] = val;
        pos >>= 1;
        while (pos) {
            seg[pos] = better(seg[pos << 1], seg[pos << 1 | 1]);
            pos >>= 1;
        }
    };

    Node answer{0, -1};

    for (int i = 0; i < n; i++) {
        cin >> c[i];
        int x = c[i];

        Node best{0, -1};

        best = better(best, query(0, x - 2));
        best = better(best, query(x, x));
        best = better(best, query(x + 2, MAXC));

        dp[i] = best.len + 1;
        parent[i] = best.idx;

        update(x, {dp[i], i});
        answer = better(answer, {dp[i], i});
    }

    vector<int> result;
    int cur = answer.idx;
    while (cur != -1) {
        result.push_back(c[cur]);
        cur = parent[cur];
    }
    reverse(result.begin(), result.end());

    cout << n - (int)result.size() << '\n';
    for (int x : result) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
