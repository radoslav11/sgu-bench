#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <set>

using namespace std;

typedef long double ld;
const ld PI = acos(-1.0);
const ld eps = 1e-9;

struct pt {
    ld x, y;
    pt() {}
    pt(ld x, ld y) : x(x), y(y) {}
};

struct line {
    ld a, b, c;
    line() {}
    line(ld a, ld b, ld c) : a(a), b(b), c(c) {}
};

vector<line> l;
ld r;
int n;

vector<pt> intersect_line_circle(line L) {
    vector<pt> res;
    ld a = L.a, b = L.b, c = L.c;
    ld d = a * a + b * b;
    if (d < eps) return res;
    ld x0 = -a * c / d;
    ld y0 = -b * c / d;
    ld dist = fabsl(c) / sqrt(d);
    if (dist > r + eps) return res;
    if (fabsl(dist - r) < eps) {
        res.push_back(pt(x0, y0));
        return res;
    }
    ld len = sqrt(r * r - dist * dist);
    ld dx = -b * len / sqrt(d);
    ld dy = a * len / sqrt(d);
    res.push_back(pt(x0 + dx, y0 + dy));
    res.push_back(pt(x0 - dx, y0 - dy));
    return res;
}

pt intersect(line L1, line L2) {
    ld d = L1.a * L2.b - L2.a * L1.b;
    if (fabsl(d) < eps) {
        return pt(1e9, 1e9);
    }
    return pt((L1.b * L2.c - L2.b * L1.c) / d, (L2.a * L1.c - L1.a * L2.c) / d);
}

bool in_circle(pt p) {
    return p.x * p.x + p.y * p.y < r * r + eps;
}

int main() {
    cin >> n >> r;
    for (int i = 0; i < n; i++) {
        ld a, b, c;
        cin >> a >> b >> c;
        l.push_back(line(a, b, c));
    }

    if (n == 1) {
        ld area = PI * r * r / 2;
        cout << fixed << setprecision(10) << area << " " << area << endl;
        return 0;
    }

    vector<ld> areas;

    for (int mask = 0; mask < (1 << n); mask++) {
        vector<pt> vertices;

        bool has_center = true;
        for (int i = 0; i < n; i++) {
            ld sign = ((mask >> i) & 1) ? 1 : -1;
            if (sign * l[i].c < -eps) {
                has_center = false;
                break;
            }
        }

        for (int i = 0; i < n; i++) {
            vector<pt> pts = intersect_line_circle(l[i]);
            for (pt p : pts) {
                bool ok = true;
                for (int j = 0; j < n; j++) {
                    ld sign = ((mask >> j) & 1) ? 1 : -1;
                    if (sign * (l[j].a * p.x + l[j].b * p.y + l[j].c) < -eps) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    vertices.push_back(p);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                pt p = intersect(l[i], l[j]);
                if (p.x > 1e8 || p.y > 1e8) continue;
                if (!in_circle(p)) continue;
                bool ok = true;
                for (int k = 0; k < n; k++) {
                    ld sign = ((mask >> k) & 1) ? 1 : -1;
                    if (sign * (l[k].a * p.x + l[k].b * p.y + l[k].c) < -eps) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    vertices.push_back(p);
                }
            }
        }

        if (vertices.empty()) {
            if (has_center) {
                areas.push_back(PI * r * r);
            }
            continue;
        }

        sort(vertices.begin(), vertices.end(), [](const pt& a, const pt& b) {
            return atan2(a.y, a.x) < atan2(b.y, b.x);
        });

        vector<pt> unique_vertices;
        for (int i = 0; i < (int)vertices.size(); i++) {
            if (i > 0) {
                if (fabsl(vertices[i].x - vertices[i-1].x) < eps && 
                    fabsl(vertices[i].y - vertices[i-1].y) < eps) {
                    continue;
                }
            }
            unique_vertices.push_back(vertices[i]);
        }

        if (unique_vertices.size() < 2) {
            continue;
        }

        ld area = 0;
        for (int i = 0; i < (int)unique_vertices.size(); i++) {
            int j = (i + 1) % unique_vertices.size();
            pt A = unique_vertices[i];
            pt B = unique_vertices[j];
            ld valA = A.x * A.x + A.y * A.y;
            ld valB = B.x * B.x + B.y * B.y;
            bool on_circle_i = fabsl(valA - r * r) < eps;
            bool on_circle_j = fabsl(valB - r * r) < eps;
            if (on_circle_i && on_circle_j) {
                ld angleA = atan2(A.y, A.x);
                ld angleB = atan2(B.y, B.x);
                ld delta = angleB - angleA;
                if (delta < 0) delta += 2 * PI;
                area += (r * r * delta);
            } else {
                area += (A.x * B.y - B.x * A.y);
            }
        }
        area = fabsl(area) / 2;
        if (area > eps) {
            areas.push_back(area);
        }
    }

    sort(areas.rbegin(), areas.rend());

    ld carlsson = 0, winnie = 0;
    for (int i = 0; i < (int)areas.size(); i++) {
        if (i % 2 == 0) {
            carlsson += areas[i];
        } else {
            winnie += areas[i];
        }
    }

    cout << fixed << setprecision(10) << carlsson << " " << winnie << endl;

    return 0;
}
