#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <set>

using namespace std;

const double eps = 1e-9;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

struct Segment {
    double x1, y1, x2, y2;
    Segment() {}
    Segment(double x1, double y1, double x2, double y2) 
        : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

Point point_on_segment(const Point& p, const Segment& seg) {
    double x1 = seg.x1, y1 = seg.y1;
    double x2 = seg.x2, y2 = seg.y2;
    double x = p.x, y = p.y;

    double v1x = x2 - x1, v1y = y2 - y1;
    double v2x = x - x1, v2y = y - y1;

    double cross = v1x * v2y - v1y * v2x;
    if (fabs(cross) > eps) {
        return Point(1e10, 1e10);
    }

    double dot = v1x * v2x + v1y * v2y;
    if (dot < -eps) {
        return Point(1e10, 1e10);
    }

    double len2 = v1x * v1x + v1y * v1y;
    if (dot > len2 + eps) {
        return Point(1e10, 1e10);
    }

    return p;
}

bool on_segment(const Point& p, const vector<Segment>& segs) {
    for (const Segment& seg : segs) {
        Point res = point_on_segment(p, seg);
        if (res.x < 1e9) {
            return true;
        }
    }
    return false;
}

Point segment_intersection(const Segment& seg1, const Segment& seg2) {
    double x1 = seg1.x1, y1 = seg1.y1, x2 = seg1.x2, y2 = seg1.y2;
    double x3 = seg2.x1, y3 = seg2.y1, x4 = seg2.x2, y2_seg2 = seg2.y2;

    double A1 = y2 - y1;
    double B1 = x1 - x2;
    double C1 = A1 * x1 + B1 * y1;

    double A2 = y2_seg2 - y3;
    double B2 = x3 - x4;
    double C2 = A2 * x3 + B2 * y3;

    double det = A1 * B2 - A2 * B1;
    if (fabs(det) < eps) {
        return Point(1e10, 1e10);
    }

    double x = (B2 * C1 - B1 * C2) / det;
    double y = (A1 * C2 - A2 * C1) / det;

    double t1 = 0.0, t2 = 0.0;
    if (fabs(seg1.x2 - seg1.x1) > eps) {
        t1 = (x - seg1.x1) / (seg1.x2 - seg1.x1);
    } else if (fabs(seg1.y2 - seg1.y1) > eps) {
        t1 = (y - seg1.y1) / (seg1.y2 - seg1.y1);
    }

    if (fabs(seg2.x2 - seg2.x1) > eps) {
        t2 = (x - seg2.x1) / (seg2.x2 - seg2.x1);
    } else if (fabs(seg2.y2 - seg2.y1) > eps) {
        t2 = (y - seg2.y1) / (seg2.y2 - seg2.y1);
    }

    if (t1 >= -eps && t1 <= 1.0 + eps && t2 >= -eps && t2 <= 1.0 + eps) {
        return Point(x, y);
    } else {
        return Point(1e10, 1e10);
    }
}

struct PointCmp {
    bool operator()(const Point& a, const Point& b) const {
        if (fabs(a.x - b.x) > eps) {
            return a.x < b.x;
        }
        if (fabs(a.y - b.y) > eps) {
            return a.y < b.y;
        }
        return false;
    }
};

int main() {
    int n;
    cin >> n;

    vector<Segment> original;
    for (int i = 0; i < n; i++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        original.push_back(Segment(x1, y1, x2, y2));
    }

    vector<vector<Segment>> sets(4);
    sets[0] = original;

    for (int i = 0; i < n; i++) {
        double x1 = original[i].x1, y1 = original[i].y1;
        double x2 = original[i].x2, y2 = original[i].y2;
        sets[1].push_back(Segment(y1, -x1, y2, -x2));
        sets[2].push_back(Segment(-x1, -y1, -x2, -y2));
        sets[3].push_back(Segment(-y1, x1, -y2, x2));
    }

    vector<Point> candidates;

    for (int s = 0; s < 4; s++) {
        for (const Segment& seg : sets[s]) {
            candidates.push_back(Point(seg.x1, seg.y1));
            candidates.push_back(Point(seg.x2, seg.y2));
        }
    }

    for (int s1 = 0; s1 < 4; s1++) {
        for (int s2 = s1 + 1; s2 < 4; s2++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    Point inter = segment_intersection(sets[s1][i], sets[s2][j]);
                    if (inter.x < 1e9) {
                        candidates.push_back(inter);
                    }
                }
            }
        }
    }

    set<Point, PointCmp> point_set;
    for (const Point& p : candidates) {
        Point p1(p.x, p.y);
        Point p2(-p.y, p.x);
        Point p3(-p.x, -p.y);
        Point p4(p.y, -p.x);

        if (on_segment(p1, sets[0]) && 
            on_segment(p2, sets[0]) && 
            on_segment(p3, sets[0]) && 
            on_segment(p4, sets[0])) {
            point_set.insert(p);
        }
    }

    vector<Point> distinct_points(point_set.begin(), point_set.end());

    double total_area = 0.0;
    int count = distinct_points.size();

    for (const Point& p : distinct_points) {
        double area = 2.0 * (p.x * p.x + p.y * p.y);
        total_area += area;
    }

    double expected_area = (count > 0) ? total_area / count : 0.0;

    cout << fixed << setprecision(10) << expected_area << endl;

    return 0;
}
