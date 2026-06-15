#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef long long ll;

struct pt {
    ll x, y;
    pt() {}
    pt(ll x, ll y) : x(x), y(y) {}
    bool operator<(const pt& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool operator==(const pt& p) const {
        return x == p.x && y == p.y;
    }
};

vector<pt> hull;

ll cross(const pt& a, const pt& b, const pt& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void add_point(pt p) {
    if (hull.size() == 0) {
        hull.push_back(p);
        return;
    }
    if (hull.size() == 1) {
        hull.push_back(p);
        return;
    }

    int n = hull.size();
    int i = 0;
    while (i < n && cross(hull[i], hull[(i+1) % n], p) >= 0) {
        i++;
    }
    if (i == n) {
        return;
    }

    int j = i;
    while (cross(hull[(j-1 + n) % n], hull[j], p) < 0) {
        j = (j - 1 + n) % n;
    }

    vector<pt> new_hull;
    new_hull.push_back(hull[i]);
    new_hull.push_back(p);
    for (int k = j; k != i; k = (k + 1) % n) {
        new_hull.push_back(hull[k]);
    }
    hull = new_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    ll x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    vector<pt> pts;
    pts.push_back(pt(x1, y1));
    pts.push_back(pt(x2, y2));
    pts.push_back(pt(x3, y3));

    sort(pts.begin(), pts.end());
    hull.clear();

    for (int i = 0; i < 3; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    int k = hull.size();
    for (int i = 2; i >= 1; i--) {
        while (hull.size() > k && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    if (hull.size() > 1 && hull[0] == hull.back()) {
        hull.pop_back();
    }

    auto compute_area = [&]() -> ll {
        if (hull.size() <= 1) {
            return 0;
        }
        ll area = 0;
        int n = hull.size();
        for (int i = 0; i < n; i++) {
            area += hull[i].x * hull[(i+1) % n].y;
            area -= hull[i].y * hull[(i+1) % n].x;
        }
        return abs(area);
    };

    ll current_area = compute_area();
    cout << current_area << '\n';

    int N;
    cin >> N;
    for (int idx = 0; idx < N; idx++) {
        ll x, y;
        cin >> x >> y;
        pt p(x, y);
        add_point(p);
        cout << compute_area() << '\n';
    }

    return 0;
}
