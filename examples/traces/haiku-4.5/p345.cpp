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
};

double polygonArea(vector<Point>& p) {
    double area = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        area += p[i].cross(p[(i + 1) % n]);
    }
    return abs(area) / 2.0;
}

// Check which side of line (a,b) the point p is on
// Returns: positive = left, negative = right, ~0 = on line
double side(Point a, Point b, Point p) {
    return (b - a).cross(p - a);
}

// Find intersection of line segment (p1,p2) with line through (a,b)
bool getIntersection(Point p1, Point p2, Point a, Point b, Point& result) {
    Point ba = b - a;
    Point p2p1 = p2 - p1;
    double denom = ba.cross(p2p1);
    
    if (abs(denom) < EPS) return false; // parallel
    
    Point ap1 = p1 - a;
    double t = ba.cross(ap1) / denom;
    
    if (t < -EPS || t > 1 + EPS) return false;
    
    result = p1 + p2p1 * t;
    return true;
}

int main(){
    ios_base::precision(10);
    ios_base::sync_with_stdio(false);
    
    int n;
    cin >> n;
    vector<Point> polygon(n);
    for (int i = 0; i < n; i++) {
        cin >> polygon[i].x >> polygon[i].y;
    }
    
    int p;
    cin >> p;
    
    while (p--) {
        Point a, b;
        cin >> a.x >> a.y >> b.x >> b.y;
        
        vector<Point> part1, part2;
        
        for (int i = 0; i < n; i++) {
            Point curr = polygon[i];
            Point next = polygon[(i + 1) % n];
            
            double s1 = side(a, b, curr);
            double s2 = side(a, b, next);
            
            // Add current point to appropriate part
            if (s1 > -EPS) {
                part1.push_back(curr);
            } else {
                part2.push_back(curr);
            }
            
            // Check for intersection
            if ((s1 > EPS && s2 < -EPS) || (s1 < -EPS && s2 > EPS)) {
                Point intersection;
                if (getIntersection(curr, next, a, b, intersection)) {
                    part1.push_back(intersection);
                    part2.push_back(intersection);
                }
            }
        }
        
        double area1 = 0, area2 = 0;
        if (part1.size() >= 3) area1 = polygonArea(part1);
        if (part2.size() >= 3) area2 = polygonArea(part2);
        
        double minArea = min(area1, area2);
        cout << fixed << setprecision(6) << minArea << "\n";
    }
    
    return 0;
}
