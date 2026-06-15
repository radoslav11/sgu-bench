#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    long long x, y;
};

struct Triangle {
    int type;
    long long rx, ry;
    long long L;
};

long long dist2(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void solve() {
    long long n;
    int m;
    if (!(cin >> n >> m)) return;
    
    vector<Triangle> tris;
    tris.reserve(m);
    long long total_area2 = 0;
    bool possible = true;
    
    for (int i = 0; i < m; ++i) {
        Point p[3];
        cin >> p[0].x >> p[0].y >> p[1].x >> p[1].y >> p[2].x >> p[2].y;
        
        long long d[3];
        d[0] = dist2(p[0], p[1]);
        d[1] = dist2(p[1], p[2]);
        d[2] = dist2(p[2], p[0]);
        
        long long sorted_d[3] = {d[0], d[1], d[2]};
        sort(sorted_d, sorted_d + 3);
        
        if (sorted_d[0] == 0 || sorted_d[0] != sorted_d[1] || sorted_d[0] + sorted_d[1] != sorted_d[2]) {
            possible = false;
        }
        
        long long L2 = sorted_d[0];
        long long L = round(sqrt(L2));
        if (L * L != L2) {
            possible = false;
        }
        
        int ra = -1;
        if (d[0] == sorted_d[2]) ra = 2;
        else if (d[1] == sorted_d[2]) ra = 0;
        else ra = 1;
        
        Point P0 = p[ra];
        Point P1 = p[(ra + 1) % 3];
        Point P2 = p[(ra + 2) % 3];
        
        long long dx = 0, dy = 0;
        if (P1.x == P0.x && P2.y == P0.y) {
            dx = P2.x - P0.x;
            dy = P1.y - P0.y;
        } else if (P2.x == P0.x && P1.y == P0.y) {
            dx = P1.x - P0.x;
            dy = P2.y - P0.y;
        } else {
            possible = false;
        }
        
        if (abs(dx) != L || abs(dy) != L) {
            possible = false;
        }
        
        int type = -1;
        if (dx > 0 && dy > 0) type = 0;
        else if (dx < 0 && dy > 0) type = 1;
        else if (dx < 0 && dy < 0) type = 2;
        else if (dx > 0 && dy < 0) type = 3;
        else possible = false;
        
        if (!possible) continue;
        
        bool inside = false;
        if (type == 0) {
            if (P0.x >= 0 && P0.y >= 0 && P0.x + P0.y + L <= n) inside = true;
        } else if (type == 1) {
            if (P0.x >= L && P0.y >= 0 && P0.x + P0.y + L <= n) inside = true;
        } else if (type == 2) {
            if (P0.x >= L && P0.y >= L && P0.x + P0.y <= n) inside = true;
        } else if (type == 3) {
            if (P0.x >= 0 && P0.y >= L && P0.x + P0.y + L <= n) inside = true;
        }
        
        if (!inside) {
            possible = false;
        }
        
        Triangle t;
        t.type = type;
        t.rx = P0.x;
        t.ry = P0.y;
        t.L = L;
        tris.push_back(t);
        total_area2 += L * L;
    }
    
    if (!possible) {
        cout << "NO\n";
        return;
    }
    
    if (total_area2 != n * n) {
        cout << "NO\n";
        return;
    }
    
    vector<vector<int>> add(n + 1), rem(n + 1);
    for (int i = 0; i < m; ++i) {
        long long x_start = 0, x_end = 0;
        if (tris[i].type == 0 || tris[i].type == 3) {
            x_start = tris[i].rx;
            x_end = tris[i].rx + tris[i].L;
        } else {
            x_start = tris[i].rx - tris[i].L;
            x_end = tris[i].rx;
        }
        if (x_start >= 0 && x_end <= n) {
            add[x_start].push_back(i);
            rem[x_end].push_back(i);
        } else {
            cout << "NO\n";
            return;
        }
    }
    
    vector<int> active;
    active.reserve(m);
    vector<int> pos_in_active(m, -1);
    
    auto remove_tri = [&](int idx) {
        int p = pos_in_active[idx];
        if (p != -1) {
            int last = active.back();
            active[p] = last;
            pos_in_active[last] = p;
            active.pop_back();
            pos_in_active[idx] = -1;
        }
    };
    
    auto add_tri = [&](int idx) {
        pos_in_active[idx] = active.size();
        active.push_back(idx);
    };
    
    for (int X = 0; X < n; ++X) {
        for (int idx : rem[X]) {
            remove_tri(idx);
        }
        for (int idx : add[X]) {
            add_tri(idx);
        }
        
        long long x2 = 2LL * X + 1;
        vector<pair<long long, long long>> intervals;
        intervals.reserve(active.size());
        
        for (int idx : active) {
            const Triangle& t = tris[idx];
            long long ymin2 = 0, ymax2 = 0;
            if (t.type == 0) {
                ymin2 = 2LL * t.ry;
                ymax2 = 2LL * (t.rx + t.ry + t.L) - x2;
            } else if (t.type == 1) {
                ymin2 = 2LL * (t.rx + t.ry) - x2;
                ymax2 = 2LL * (t.ry + t.L);
            } else if (t.type == 2) {
                ymin2 = 2LL * (t.rx + t.ry - t.L) - x2;
                ymax2 = 2LL * t.ry;
            } else if (t.type == 3) {
                ymin2 = 2LL * (t.ry - t.L);
                ymax2 = x2 - 2LL * (t.rx - t.ry);
            }
            intervals.push_back({ymin2, ymax2});
        }
        
        sort(intervals.begin(), intervals.end());
        for (size_t j = 1; j < intervals.size(); ++j) {
            if (intervals[j].first < intervals[j-1].second) {
                cout << "NO\n";
                return;
            }
        }
    }
    
    cout << "YES\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
