#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

struct Edge {
    int a, b;
    int x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<Point> p(N);
    for (int i = 0; i < N; ++i) {
        cin >> p[i].x >> p[i].y;
    }

    vector<int> ids(N);
    iota(ids.begin(), ids.end(), 0);

    sort(ids.begin(), ids.end(), [&](int a, int b) {
        if (p[a].x != p[b].x) return p[a].x < p[b].x;
        return p[a].y < p[b].y;
    });

    for (int i = 1; i < N; ++i) {
        if (p[ids[i]].x == p[ids[i - 1]].x && p[ids[i]].y == p[ids[i - 1]].y) {
            cout << 0 << '\n';
            return 0;
        }
    }

    vector<array<int, 2>> adj(N);
    vector<int> deg(N, 0);

    vector<Edge> horizontal, vertical;
    long long answer = 0;

    auto add_edge = [&](int a, int b, bool hor) {
        if (deg[a] >= 2 || deg[b] >= 2) {
            cout << 0 << '\n';
            exit(0);
        }

        adj[a][deg[a]++] = b;
        adj[b][deg[b]++] = a;

        Edge e;
        e.a = a;
        e.b = b;
        e.x1 = p[a].x;
        e.y1 = p[a].y;
        e.x2 = p[b].x;
        e.y2 = p[b].y;

        if (e.x1 > e.x2) swap(e.x1, e.x2);
        if (e.y1 > e.y2) swap(e.y1, e.y2);

        answer += abs(p[a].x - p[b].x) + abs(p[a].y - p[b].y);

        if (hor) horizontal.push_back(e);
        else vertical.push_back(e);
    };

    sort(ids.begin(), ids.end(), [&](int a, int b) {
        if (p[a].y != p[b].y) return p[a].y < p[b].y;
        return p[a].x < p[b].x;
    });

    for (int i = 0; i < N;) {
        int j = i;
        while (j < N && p[ids[j]].y == p[ids[i]].y) ++j;

        if ((j - i) % 2 == 1) {
            cout << 0 << '\n';
            return 0;
        }

        for (int k = i; k < j; k += 2) {
            add_edge(ids[k], ids[k + 1], true);
        }

        i = j;
    }

    sort(ids.begin(), ids.end(), [&](int a, int b) {
        if (p[a].x != p[b].x) return p[a].x < p[b].x;
        return p[a].y < p[b].y;
    });

    for (int i = 0; i < N;) {
        int j = i;
        while (j < N && p[ids[j]].x == p[ids[i]].x) ++j;

        if ((j - i) % 2 == 1) {
            cout << 0 << '\n';
            return 0;
        }

        for (int k = i; k < j; k += 2) {
            add_edge(ids[k], ids[k + 1], false);
        }

        i = j;
    }

    for (int i = 0; i < N; ++i) {
        if (deg[i] != 2) {
            cout << 0 << '\n';
            return 0;
        }
    }

    vector<char> used(N, 0);
    int visited = 0;
    stack<int> st;
    st.push(0);
    used[0] = 1;

    while (!st.empty()) {
        int v = st.top();
        st.pop();
        ++visited;

        for (int k = 0; k < 2; ++k) {
            int to = adj[v][k];
            if (!used[to]) {
                used[to] = 1;
                st.push(to);
            }
        }
    }

    if (visited != N) {
        cout << 0 << '\n';
        return 0;
    }

    for (const Edge &h : horizontal) {
        for (const Edge &v : vertical) {
            if (v.x1 >= h.x1 && v.x1 <= h.x2 &&
                h.y1 >= v.y1 && h.y1 <= v.y2) {

                bool commonEndpoint = false;

                if (p[h.a].x == v.x1 && p[h.a].y == h.y1 &&
                    (h.a == v.a || h.a == v.b)) {
                    commonEndpoint = true;
                }

                if (p[h.b].x == v.x1 && p[h.b].y == h.y1 &&
                    (h.b == v.a || h.b == v.b)) {
                    commonEndpoint = true;
                }

                if (!commonEndpoint) {
                    cout << 0 << '\n';
                    return 0;
                }
            }
        }
    }

    cout << answer << '\n';
    return 0;
}
