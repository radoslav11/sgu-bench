#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

struct Fenwick {
    int n;
    vector<int> bit, touched;

    Fenwick(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        bit.assign(n + 2, 0);
        touched.clear();
    }

    void add(int idx, int val) {
        for (int i = idx; i <= n; i += i & -i) {
            if (bit[i] == 0) touched.push_back(i);
            bit[i] += val;
        }
    }

    int sumPrefix(int idx) const {
        int res = 0;
        for (int i = idx; i > 0; i -= i & -i) res += bit[i];
        return res;
    }

    int rangeSum(int l, int r) const {
        if (l > r) return 0;
        l = max(l, 1);
        r = min(r, n);
        if (l > r) return 0;
        return sumPrefix(r) - sumPrefix(l - 1);
    }

    void clearTouched() {
        for (int i : touched) bit[i] = 0;
        touched.clear();
    }
};

int n, maxY;
vector<Point> p;
Fenwick fw;

long long countCrossNE(const vector<int>& left, const vector<int>& right) {
    int m = (int)p.size();
    vector<int> up(m, maxY + 1), low(m, 0);

    vector<int> L = left;
    sort(L.begin(), L.end(), [&](int a, int b) {
        if (p[a].x != p[b].x) return p[a].x > p[b].x;
        return p[a].y < p[b].y;
    });

    set<int> ys;
    for (int i = 0; i < (int)L.size();) {
        int j = i;
        while (j < (int)L.size() && p[L[j]].x == p[L[i]].x) {
            ys.insert(p[L[j]].y);
            j++;
        }
        for (int k = i; k < j; k++) {
            auto it = ys.upper_bound(p[L[k]].y);
            if (it != ys.end()) up[L[k]] = *it;
        }
        i = j;
    }

    vector<int> R = right;
    sort(R.begin(), R.end(), [&](int a, int b) {
        if (p[a].x != p[b].x) return p[a].x < p[b].x;
        return p[a].y < p[b].y;
    });

    ys.clear();
    for (int i = 0; i < (int)R.size();) {
        int j = i;
        while (j < (int)R.size() && p[R[j]].x == p[R[i]].x) {
            ys.insert(p[R[j]].y);
            j++;
        }
        for (int k = i; k < j; k++) {
            auto it = ys.lower_bound(p[R[k]].y);
            if (it != ys.begin()) {
                --it;
                low[R[k]] = *it;
            }
        }
        i = j;
    }

    sort(L.begin(), L.end(), [&](int a, int b) {
        return p[a].y < p[b].y;
    });

    sort(R.begin(), R.end(), [&](int a, int b) {
        if (low[a] != low[b]) return low[a] < low[b];
        return p[a].y < p[b].y;
    });

    long long res = 0;
    int ptr = 0;

    for (int id : L) {
        while (ptr < (int)R.size() && low[R[ptr]] < p[id].y) {
            fw.add(p[R[ptr]].y, 1);
            ptr++;
        }

        res += fw.rangeSum(p[id].y + 1, up[id] - 1);
    }

    fw.clearTouched();
    return res;
}

long long solveDC(const vector<int>& ids, int lx, int rx) {
    if (lx == rx || ids.size() <= 1) return 0;

    int mid = (lx + rx) / 2;
    vector<int> left, right;
    left.reserve(ids.size());
    right.reserve(ids.size());

    for (int id : ids) {
        if (p[id].x <= mid) left.push_back(id);
        else right.push_back(id);
    }

    long long res = 0;
    if (!left.empty()) res += solveDC(left, lx, mid);
    if (!right.empty()) res += solveDC(right, mid + 1, rx);
    if (!left.empty() && !right.empty()) res += countCrossNE(left, right);

    return res;
}

long long countNE() {
    vector<int> ids(n);
    iota(ids.begin(), ids.end(), 0);

    int maxX = 0;
    for (auto &pt : p) maxX = max(maxX, pt.x);

    fw.init(maxY + 2);
    return solveDC(ids, 1, maxX);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    vector<pair<long long, long long>> orig(n);
    vector<long long> xs, ys;

    for (int i = 0; i < n; i++) {
        cin >> orig[i].first >> orig[i].second;
        xs.push_back(orig[i].first);
        ys.push_back(orig[i].second);
    }

    long long ans = 0;

    {
        map<long long, vector<long long>> byX, byY;
        for (auto [x, y] : orig) {
            byX[x].push_back(y);
            byY[y].push_back(x);
        }

        for (auto &[x, v] : byX) {
            sort(v.begin(), v.end());
            ans += (long long)v.size() - 1;
        }

        for (auto &[y, v] : byY) {
            sort(v.begin(), v.end());
            ans += (long long)v.size() - 1;
        }
    }

    auto compressAndSolve = [&](bool negateY) -> long long {
        vector<long long> cx, cy;
        cx.reserve(n);
        cy.reserve(n);

        for (auto [x, y] : orig) {
            cx.push_back(x);
            cy.push_back(negateY ? -y : y);
        }

        sort(cx.begin(), cx.end());
        cx.erase(unique(cx.begin(), cx.end()), cx.end());

        sort(cy.begin(), cy.end());
        cy.erase(unique(cy.begin(), cy.end()), cy.end());

        p.assign(n, {0, 0});

        for (int i = 0; i < n; i++) {
            long long yy = negateY ? -orig[i].second : orig[i].second;

            p[i].x = int(lower_bound(cx.begin(), cx.end(), orig[i].first) - cx.begin()) + 1;
            p[i].y = int(lower_bound(cy.begin(), cy.end(), yy) - cy.begin()) + 1;
        }

        maxY = (int)cy.size();
        return countNE();
    };

    ans += compressAndSolve(false);
    ans += compressAndSolve(true);

    cout << ans << '\n';

    return 0;
}
