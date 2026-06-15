#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

typedef double db;
const db PI = acos(-1.0);
const db EPS = 1e-9;

struct Point {
    db x, y;
};

db dist(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

struct RoutePart {
    char type;
    Point p1, p2;
};

Point get_point(Point c, db r, db theta) {
    return {c.x + r * cos(theta), c.y + r * sin(theta)};
}

db normalize_angle(db a) {
    while (a <= -PI) a += 2 * PI;
    while (a > PI) a -= 2 * PI;
    return a;
}

db arc_len(Point c, db r, db a1, db a2) {
    db da = abs(normalize_angle(a2 - a1));
    if (da > PI) da = 2 * PI - da;
    return r * da;
}

db get_best_theta2(Point C, db R, Point B, db t1) {
    db best_t2 = t1, min_val = 1e18;
    for (int i = 0; i <= 100; ++i) {
        db t2 = t1 - PI + 2.0 * PI * i / 100.0;
        db val = arc_len(C, R, t1, t2) + dist(get_point(C, R, t2), B);
        if (val < min_val) { min_val = val; best_t2 = t2; }
    }
    db step = 2.0 * PI / 100.0;
    for (int iter = 0; iter < 40; ++iter) {
        db t2a = best_t2 - step / 2, t2b = best_t2 + step / 2;
        if (arc_len(C, R, t1, t2a) + dist(get_point(C, R, t2a), B) < 
            arc_len(C, R, t1, t2b) + dist(get_point(C, R, t2b), B)) best_t2 = t2a;
        else best_t2 = t2b;
        step /= 2.0;
    }
    return best_t2;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    Point A, B, C; db R, D;
    if (!(cin >> A.x >> A.y >> B.x >> B.y >> C.x >> C.y >> R >> D)) return 0;
    db min_dist = 1e18;
    vector<RoutePart> best_route;
    auto update = [&](db d, const vector<RoutePart>& r) {
        if (d < min_dist) { min_dist = d; best_route = r; }
    };
    db dx = B.x - A.x, dy = B.y - A.y, L = dist(A, B);
    db a = dx * dx + dy * dy, b = 2 * ((A.x - C.x) * dx + (A.y - C.y) * dy);
    db c = pow(A.x - C.x, 2) + pow(A.y - C.y, 2) - R * R, disc = b * b - 4 * a * c;
    db inside = 0;
    if (disc > 0) {
        db r1 = (-b - sqrt(disc)) / (2 * a), r2 = (-b + sqrt(disc)) / (2 * a);
        db t_s = max(0.0, min(1.0, r1)), t_e = max(0.0, min(1.0, r2));
        if (t_e > t_s) inside = (t_e - t_s) * L;
    }
    if (inside <= D + 1e-7) update(L, {{'S', A, B}});
    for (int i = 0; i < 360; ++i) {
        db t0 = 2.0 * PI * i / 360.0;
        db t2_0 = get_best_theta2(C, R, B, t0);
        update(dist(A, get_point(C, R, t0)) + arc_len(C, R, t0, t2_0) + dist(get_point(C, R, t2_0), B),
               {{'S', A, get_point(C, R, t0)}, {'A', get_point(C, R, t0), get_point(C, R, t2_0)}, {'S', get_point(C, R, t2_0), B}});
        if (D > 1e-7) {
            db d = min(D, 2.0 * R), alpha = 2.0 * asin(min(1.0, d / (2.0 * R)));
            for (db s : {-1.0, 1.0}) {
                db t1 = t0 + s * alpha;
                db t2_1 = get_best_theta2(C, R, B, t1);
                update(dist(A, get_point(C, R, t0)) + d + arc_len(C, R, t1, t2_1) + dist(get_point(C, R, t2_1), B),
                       {{'S', A, get_point(C, R, t0)}, {'S', get_point(C, R, t0), get_point(C, R, t1)}, {'A', get_point(C, R, t1), get_point(C, R, t2_1)}, {'S', get_point(C, R, t2_1), B}});
            }
        }
    }
    vector<RoutePart> filtered;
    for (auto& p : best_route) if (dist(p.p1, p.p2) > 1e-8) filtered.push_back(p);
    cout << filtered.size() << endl;
    cout << fixed << setprecision(8);
    for (auto& p : filtered) cout << p.type << " " << p.p1.x << " " << p.p1.y << " " << p.p2.x << " " << p.p2.y << endl;
    return 0;
}

