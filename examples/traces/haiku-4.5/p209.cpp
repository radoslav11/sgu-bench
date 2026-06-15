#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double MIN_AREA = 1e-8;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(double t) const { return Point(x * t, y * t); }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
    double dot(const Point& p) const { return x * p.x + y * p.y; }
    bool operator<(const Point& p) const {
        return x < p.x - EPS || (fabs(x - p.x) <= EPS && y < p.y - EPS);
    }
    bool operator==(const Point& p) const {
        return fabs(x - p.x) <= EPS && fabs(y - p.y) <= EPS;
    }
};

struct Line {
    double a, b, c;
    Line(double a = 0, double b = 0, double c = 0) : a(a), b(b), c(c) {}
};

bool intersection(const Line& l1, const Line& l2, Point& p) {
    double det = l1.a * l2.b - l1.b * l2.a;
    if (fabs(det) < EPS) return false;
    p.x = (l1.b * l2.c - l1.c * l2.b) / det;
    p.y = (l1.c * l2.a - l1.a * l2.c) / det;
    return true;
}

double polygonArea(vector<Point>& pts) {
    if (pts.size() < 3) return 0;
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        Point c = (a + b) * 0.5;
        double ang_a = atan2(a.y - c.y, a.x - c.x);
        double ang_b = atan2(b.y - c.y, b.x - c.x);
        return ang_a < ang_b;
    });
    
    double area = 0;
    for (int i = 0; i < pts.size(); i++) {
        area += pts[i].cross(pts[(i + 1) % pts.size()]);
    }
    return fabs(area) / 2;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Line> lines(n);
    for (int i = 0; i < n; i++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        lines[i].a = y2 - y1;
        lines[i].b = x1 - x2;
        lines[i].c = x2 * y1 - x1 * y2;
        double len = sqrt(lines[i].a * lines[i].a + lines[i].b * lines[i].b);
        lines[i].a /= len;
        lines[i].b /= len;
        lines[i].c /= len;
    }
    
    vector<Point> pts;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Point p;
            if (intersection(lines[i], lines[j], p)) {
                bool found = false;
                for (auto& pt : pts) {
                    if (pt == p) { found = true; break; }
                }
                if (!found) pts.push_back(p);
            }
        }
    }
    
    vector<double> areas;
    int m = pts.size();
    
    for (int mask = 0; mask < (1 << m); mask++) {
        if (__builtin_popcount(mask) < 3) continue;
        vector<Point> poly;
        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) poly.push_back(pts[i]);
        }
        double area = polygonArea(poly);
        if (area > MIN_AREA) {
            areas.push_back(area);
        }
    }
    
    sort(areas.begin(), areas.end());
    areas.erase(unique(areas.begin(), areas.end(), [](double a, double b) {
        return fabs(a - b) < MIN_AREA;
    }), areas.end());
    
    cout << areas.size() << "\n";
    cout << fixed << setprecision(4);
    for (double a : areas) {
        cout << a << "\n";
    }
    
    return 0;
}
