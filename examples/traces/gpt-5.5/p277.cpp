#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Point {
    int64 x, y;
};

i128 cross(const Point& a, const Point& b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

i128 cross(const Point& a, const Point& b, const Point& c) {
    return (i128)(b.x - a.x) * (c.y - a.y) - (i128)(b.y - a.y) * (c.x - a.x);
}

class Chain {
    // lower = true  -> lower hull, remove turns with cross <= 0
    // lower = false -> upper hull, remove turns with cross >= 0
    bool lower;
    map<int64, Point> hull;
    i128 sum = 0; // sum of cross products of consecutive points from left to right

    bool bad(const Point& a, const Point& b, const Point& c) const {
        i128 cr = cross(a, b, c);
        return lower ? (cr <= 0) : (cr >= 0);
    }

    void erase_it(map<int64, Point>::iterator it) {
        auto prv = it, nxt = it;
        bool hasPrev = false, hasNext = false;

        if (it != hull.begin()) {
            --prv;
            hasPrev = true;
            sum -= cross(prv->second, it->second);
        }

        ++nxt;
        if (nxt != hull.end()) {
            hasNext = true;
            sum -= cross(it->second, nxt->second);
        }

        if (hasPrev && hasNext) {
            sum += cross(prv->second, nxt->second);
        }

        hull.erase(it);
    }

    map<int64, Point>::iterator insert_raw(Point p) {
        auto it = hull.lower_bound(p.x);
        bool hasPrev = false, hasNext = false;
        map<int64, Point>::iterator prv;

        if (it != hull.begin()) {
            prv = prev(it);
            hasPrev = true;
            sum += cross(prv->second, p);
        }

        if (it != hull.end()) {
            hasNext = true;
            sum += cross(p, it->second);
        }

        if (hasPrev && hasNext) {
            sum -= cross(prv->second, it->second);
        }

        return hull.emplace(p.x, p).first;
    }

public:
    Chain(bool isLower) : lower(isLower) {}

    void add(Point p) {
        auto same = hull.find(p.x);
        if (same != hull.end()) {
            if (lower) {
                if (same->second.y <= p.y) return;
            } else {
                if (same->second.y >= p.y) return;
            }
            erase_it(same);
        }

        if (hull.size() >= 2) {
            auto it = hull.lower_bound(p.x);
            if (it != hull.begin() && it != hull.end()) {
                auto prv = prev(it);
                i128 cr = cross(prv->second, p, it->second);

                if (lower) {
                    if (cr <= 0) return;
                } else {
                    if (cr >= 0) return;
                }
            }
        }

        auto it = insert_raw(p);

        while (it != hull.begin()) {
            auto p1 = prev(it);
            if (p1 == hull.begin()) break;
            auto p0 = prev(p1);

            if (!bad(p0->second, p1->second, it->second)) break;
            erase_it(p1);
        }

        while (true) {
            auto p1 = next(it);
            if (p1 == hull.end()) break;
            auto p2 = next(p1);
            if (p2 == hull.end()) break;

            if (!bad(it->second, p1->second, p2->second)) break;
            erase_it(p1);
        }
    }

    i128 get_sum() const {
        return sum;
    }

    Point left_point() const {
        return hull.begin()->second;
    }

    Point right_point() const {
        return hull.rbegin()->second;
    }
};

void print_i128(i128 x) {
    if (x < 0) {
        cout << '-';
        x = -x;
    }

    if (x == 0) {
        cout << '0';
        return;
    }

    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }

    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Chain lower(true), upper(false);

    for (int i = 0; i < 3; ++i) {
        Point p;
        cin >> p.x >> p.y;
        lower.add(p);
        upper.add(p);
    }

    int N;
    cin >> N;

    while (N--) {
        Point p;
        cin >> p.x >> p.y;

        lower.add(p);
        upper.add(p);

        Point lL = lower.left_point();
        Point lR = lower.right_point();
        Point uL = upper.left_point();
        Point uR = upper.right_point();

        i128 area2 = lower.get_sum()
                   + cross(lR, uR)
                   - upper.get_sum()
                   + cross(uL, lL);

        if (area2 < 0) area2 = -area2;

        print_i128(area2);
        cout << '\n';
    }

    return 0;
}
