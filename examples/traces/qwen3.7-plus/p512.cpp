#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    int x, y, id;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return id < other.id;
    }
};

void find_cross(int L, int mid, int R, const vector<Point>& pts, vector<pair<int, int>>& friends) {
    vector<Point> L_pts, R_pts;
    for (int i = L; i <= mid; ++i) L_pts.push_back(pts[i]);
    for (int i = mid + 1; i <= R; ++i) R_pts.push_back(pts[i]);

    sort(L_pts.begin(), L_pts.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });
    sort(R_pts.begin(), R_pts.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });

    vector<Point> active_L, active_R;
    int ptr_L = 0;

    for (const auto& q : R_pts) {
        while (ptr_L < (int)L_pts.size() && L_pts[ptr_L].y <= q.y) {
            Point pt = L_pts[ptr_L];
            while (!active_L.empty() && pt.x >= active_L.back().x) {
                active_L.pop_back();
            }
            active_L.push_back(pt);
            ptr_L++;
        }

        for (int i = (int)active_L.size() - 1; i >= 0; --i) {
            Point p = active_L[i];
            if (p.x > q.x) break;
            
            auto it = lower_bound(active_R.begin(), active_R.end(), p.y, [](const Point& a, int val) {
                return a.y < val;
            });
            
            if (it == active_R.end() || it->x > q.x) {
                int u = p.id, v = q.id;
                if (u > v) swap(u, v);
                friends.push_back({u, v});
            }
        }

        while (!active_R.empty() && active_R.back().x >= q.x) {
            active_R.pop_back();
        }
        active_R.push_back(q);
    }
}

void solve(int L, int R, const vector<Point>& pts, vector<pair<int, int>>& friends) {
    if (L >= R) return;
    int mid = L + (R - L) / 2;
    solve(L, mid, pts, friends);
    solve(mid + 1, R, pts, friends);
    find_cross(L, mid, R, pts, friends);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].x >> pts[i].y;
        pts[i].id = i;
    }

    vector<pair<int, int>> friends;
    
    sort(pts.begin(), pts.end());
    solve(0, n - 1, pts, friends);

    vector<Point> pts_neg = pts;
    for (auto& p : pts_neg) {
        p.y = -p.y;
    }
    sort(pts_neg.begin(), pts_neg.end());
    solve(0, n - 1, pts_neg, friends);

    sort(friends.begin(), friends.end());
    friends.erase(unique(friends.begin(), friends.end()), friends.end());

    cout << friends.size() << "\n";

    return 0;
}
