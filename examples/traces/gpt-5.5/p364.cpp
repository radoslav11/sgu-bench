#include <bits/stdc++.h>
using namespace std;

struct Platform {
    int l, r, h;
};

struct Res {
    int c;
    int t;
};

const int INF = 1e9;

Res bestRes(Res a, Res b) {
    if (a.c != b.c) return (a.c < b.c) ? a : b;
    return (a.t < b.t) ? a : b;
}

int N, s;
int X, H, A, B;
int M;
vector<Platform> p;

map<tuple<int,int,int>, Res> memo;

int fallTo(int x, int h) {
    int id = -1;
    int bestH = -1;
    for (int i = 0; i < M; i++) {
        if (p[i].h < h && p[i].l < x && x < p[i].r) {
            if (p[i].h > bestH) {
                bestH = p[i].h;
                id = i;
            }
        }
    }
    return id;
}

bool homeOnPlatform(int i) {
    return p[i].h == B && p[i].l <= A && A <= p[i].r;
}

Res solve(int id, int x, int dir) {
    auto key = make_tuple(id, x, dir);
    if (memo.count(key)) return memo[key];

    Res ans{INF, INF};

    const Platform &cur = p[id];

    if (homeOnPlatform(id)) {
        if (dir == 1 && x <= A && A <= cur.r) {
            ans = bestRes(ans, {0, A - x});
        }
        if (dir == -1 && cur.l <= A && A <= x) {
            ans = bestRes(ans, {0, x - A});
        }

        // Put a stopped lemming immediately at current point and reverse.
        if (dir == 1 && cur.l <= A && A <= x) {
            ans = bestRes(ans, {1, x - A});
        }
        if (dir == -1 && x <= A && A <= cur.r) {
            ans = bestRes(ans, {1, A - x});
        }
    }

    // Continue forward to the end of the platform.
    {
        int ex = (dir == 1 ? cur.r : cur.l);
        int walk = abs(ex - x);
        int nxt = fallTo(ex, cur.h);
        if (nxt != -1) {
            Res sub = solve(nxt, ex, dir);
            if (sub.c < INF) {
                ans = bestRes(ans, {sub.c, walk + (cur.h - p[nxt].h) + sub.t});
            }
        }
    }

    // Stop a lemming here, reverse direction, then go to the opposite end.
    {
        int ndir = -dir;
        int ex = (ndir == 1 ? cur.r : cur.l);
        int walk = abs(ex - x);
        int nxt = fallTo(ex, cur.h);
        if (nxt != -1) {
            Res sub = solve(nxt, ex, ndir);
            if (sub.c < INF) {
                ans = bestRes(ans, {sub.c + 1, walk + (cur.h - p[nxt].h) + sub.t});
            }
        }
    }

    return memo[key] = ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> s;
    cin >> X >> H >> A >> B;
    cin >> M;

    p.resize(M);
    for (int i = 0; i < M; i++) {
        cin >> p[i].l >> p[i].r >> p[i].h;
    }

    int start = fallTo(X, H);
    if (start == -1) {
        cout << "0 0\n";
        return 0;
    }

    Res ans = solve(start, X, 1);

    if (ans.c >= INF || ans.c >= N) {
        cout << "0 0\n";
        return 0;
    }

    int K = N - ans.c;
    int T = (N - 1) * s + (H - p[start].h) + ans.t;

    cout << K << ' ' << T << '\n';

    return 0;
}
