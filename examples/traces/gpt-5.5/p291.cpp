#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int id;
    bool operator>(const Node& other) const {
        return key > other.key;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q, c, n;
    long long t;
    cin >> q >> c >> n >> t;

    int total = q * c;
    const int INF = INT_MAX;

    vector<int> best(total, INF);
    vector<int> owner(total, -1);
    vector<long long> ans(n, 0);

    priority_queue<Node, vector<Node>, greater<Node>> pq;

    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        --x; --y;
        int id = x * c + y;
        int key = i;
        best[id] = key;
        pq.push({key, id});
    }

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        int id = cur.id;
        int key = cur.key;

        if (owner[id] != -1 || best[id] != key) continue;

        int dist = key / n;
        int cult = key % n;

        owner[id] = cult;
        ans[cult]++;

        if (dist >= t) continue;

        int x = id / c;
        int y = id % c;

        int nextKey = (dist + 1) * n + cult;

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (nx < 0 || nx >= q || ny < 0 || ny >= c) continue;

            int nid = nx * c + ny;

            if (owner[nid] == -1 && nextKey < best[nid]) {
                best[nid] = nextKey;
                pq.push({nextKey, nid});
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}
