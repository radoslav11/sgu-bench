#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(double t) const { return Point(x * t, y * t); }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
    double dot(const Point& p) const { return x * p.x + y * p.y; }
    double norm() const { return sqrt(x * x + y * y); }
};

double ccw(Point a, Point b, Point c) {
    return (b - a).cross(c - a);
}

Point convexHull(vector<Point>& pts) {
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (abs(a.x - b.x) < EPS && a.y < b.y);
    });
    Point p = pts[0];
    sort(pts.begin() + 1, pts.end(), [&](const Point& a, const Point& b) {
        double v = ccw(p, a, b);
        if (abs(v) < EPS) return (a - p).norm() < (b - p).norm();
        return v > 0;
    });
    return p;
}

bool segmentIntersection(Point p1, Point p2, Point p3, Point p4, Point& intersection) {
    double a1 = p2.y - p1.y;
    double b1 = p1.x - p2.x;
    double c1 = a1 * p1.x + b1 * p1.y;
    
    double a2 = p4.y - p3.y;
    double b2 = p3.x - p4.x;
    double c2 = a2 * p3.x + b2 * p3.y;
    
    double det = a1 * b2 - a2 * b1;
    if (abs(det) < EPS) return false;
    
    intersection.x = (b2 * c1 - b1 * c2) / det;
    intersection.y = (a1 * c2 - a2 * c1) / det;
    return true;
}

bool pointInTriangle(Point p, Point a, Point b, Point c) {
    double d1 = ccw(a, b, p);
    double d2 = ccw(b, c, p);
    double d3 = ccw(c, a, p);
    bool neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(neg && pos);
}

bool pointInConvexPolygon(Point p, vector<Point>& poly) {
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        if (ccw(poly[i], poly[(i+1)%n], p) < -EPS) return false;
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<Point> poly(n);
    for (int i = 0; i < n; i++) {
        cin >> poly[i].x >> poly[i].y;
    }
    
    convexHull(poly);
    
    int m;
    cin >> m;
    
    cout << fixed << setprecision(2);
    
    for (int i = 0; i < m; i++) {
        Point a, b;
        cin >> a.x >> a.y >> b.x >> b.y;
        
        vector<Point> inside;
        if (pointInConvexPolygon(a, poly)) inside.push_back(a);
        if (pointInConvexPolygon(b, poly)) inside.push_back(b);
        
        for (int j = 0; j < n; j++) {
            Point inter;
            if (segmentIntersection(a, b, poly[j], poly[(j+1)%n], inter)) {
                double t = (inter - a).norm() / (b - a).norm();
                if (t > EPS && t < 1 - EPS) {
                    inside.push_back(inter);
                }
            }
        }
        
        if (inside.size() >= 2) {
            sort(inside.begin(), inside.end(), [&](const Point& p1, const Point& p2) {
                return (p1 - a).norm() < (p2 - a).norm();
            });
            double len = 0;
            for (int j = 0; j + 1 < inside.size(); j++) {
                len += (inside[j+1] - inside[j]).norm();
            }
            cout << len << "\n";
        } else {
            cout << "0\n";
        }
    }
    
    return 0;
}
