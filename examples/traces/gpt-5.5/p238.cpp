#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using boost::multiprecision::cpp_int;

int N;
vector<vector<int>> g;
vector<cpp_int> fact;

pair<string, cpp_int> rooted(int u, int p) {
    vector<pair<string, cpp_int>> children;
    for (int v : g[u]) {
        if (v != p) children.push_back(rooted(v, u));
    }

    sort(children.begin(), children.end(),
         [](const auto& a, const auto& b) {
             return a.first < b.first;
         });

    string repr = "(";
    cpp_int aut = 1;

    for (int i = 0; i < (int)children.size(); ) {
        int j = i;
        while (j < (int)children.size() && children[j].first == children[i].first) j++;

        int cnt = j - i;
        aut *= fact[cnt];

        for (int k = i; k < j; k++) {
            aut *= children[k].second;
            repr += children[k].first;
        }

        i = j;
    }

    repr += ")";
    return {repr, aut};
}

vector<int> find_centers(int total) {
    vector<int> deg(total);
    queue<int> q;

    for (int i = 0; i < total; i++) {
        deg[i] = (int)g[i].size();
        if (deg[i] <= 1) q.push(i);
    }

    int remaining = total;
    while (remaining > 2) {
        int sz = q.size();
        remaining -= sz;

        while (sz--) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                deg[v]--;
                if (deg[v] == 1) q.push(v);
            }
        }
    }

    vector<int> centers;
    while (!q.empty()) {
        centers.push_back(q.front());
        q.pop();
    }

    return centers;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;

    int total = N + 1;
    g.assign(total, {});
    vector<int> parent(N + 1, -1);

    for (int i = 1; i <= N; i++) {
        int c;
        cin >> c;

        for (int j = 0; j < c; j++) {
            int x;
            cin >> x;

            g[i].push_back(x);
            g[x].push_back(i);
            parent[x] = i;
        }
    }

    for (int i = 1; i <= N; i++) {
        if (parent[i] == -1) {
            g[0].push_back(i);
            g[i].push_back(0);
        }
    }

    fact.assign(total + 1, 1);
    for (int i = 1; i <= total; i++) {
        fact[i] = fact[i - 1] * i;
    }

    vector<int> centers = find_centers(total);

    cpp_int automorphisms;

    if (centers.size() == 1) {
        automorphisms = rooted(centers[0], -1).second;
    } else {
        int a = centers[0];
        int b = centers[1];

        auto left = rooted(a, b);
        auto right = rooted(b, a);

        automorphisms = left.second * right.second;
        if (left.first == right.first) {
            automorphisms *= 2;
        }
    }

    cpp_int answer = fact[total] / automorphisms;
    cout << answer << '\n';

    return 0;
}
