#include <bits/stdc++.h>
using namespace std;

struct Item {
    string t;
    int v[3];
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int need[3];
    cin >> need[0] >> need[1] >> need[2];

    int N;
    cin >> N;

    vector<Item> a(N);
    for (int i = 0; i < N; ++i) {
        cin >> a[i].t;
        for (int j = 0; j < 3; ++j)
            a[i].v[j] = (a[i].t[j] == 'B' ? 2 : 1);
    }

    vector<int> ord(N);
    iota(ord.begin(), ord.end(), 0);

    sort(ord.begin(), ord.end(), [&](int x, int y) {
        int sx = a[x].v[0] + a[x].v[1] + a[x].v[2];
        int sy = a[y].v[0] + a[y].v[1] + a[y].v[2];
        return sx > sy;
    });

    string ans(N, '?');
    int got[3] = {0, 0, 0};

    for (int id : ord) {
        int best = -1;
        int bestScore = -1;

        for (int j = 0; j < 3; ++j) {
            int deficit = max(0, need[j] - got[j]);
            int score = min(deficit, a[id].v[j]) * 1000 + deficit;
            if (score > bestScore) {
                bestScore = score;
                best = j;
            }
        }

        ans[id] = "POS"[best];
        got[best] += a[id].v[best];
    }

    bool changed = true;
    int iter = 0;

    while (changed && iter++ < 50) {
        changed = false;

        for (int i = 0; i < N; ++i) {
            int cur = (ans[i] == 'P' ? 0 : ans[i] == 'O' ? 1 : 2);

            for (int to = 0; to < 3; ++to) {
                if (to == cur) continue;

                if (got[to] >= need[to]) continue;
                if (got[cur] - a[i].v[cur] < need[cur]) continue;

                int before = max(0, need[to] - got[to]);
                int after = max(0, need[to] - (got[to] + a[i].v[to]));

                if (after < before) {
                    got[cur] -= a[i].v[cur];
                    got[to] += a[i].v[to];
                    ans[i] = "POS"[to];
                    changed = true;
                    break;
                }
            }
        }
    }

    if (got[0] >= need[0] && got[1] >= need[1] && got[2] >= need[2]) {
        cout << ans << '\n';
    } else {
        cout << "no solution\n";
    }

    return 0;
}
