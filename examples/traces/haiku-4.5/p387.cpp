#include<bits/stdc++.h>
using std/::cout;
using std/::cin;
using std/::vector;

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    Point operator*(double t) const { return Point(x*t, y*t); }
    double dot(const Point& p) const { return x*p.x + y*p.y; }
    double cross(const Point& p) const { return x*p.y - y*p.x; }
};

bool pointOnSegment(Point p, Point a, Point b) {
    Point ap = p - a, ab = b - a;
    if(abs(ap.cross(ab)) > 1e-9) return false;
    return ap.dot(ab) > -1e-9 && ap.dot(ab) < ab.dot(ab) + 1e-9;
}

bool pointCoveredBySegment(Point p, Point a, Point b) {
    return pointOnSegment(p, a, b);
}

bool isValidSquare(Point base, vector<pair<Point,Point>>& segments) {
    Point b1 = base;
    Point b2 = Point(-base.y, base.x);
    Point b3 = Point(-base.x, -base.y);
    Point b4 = Point(base.y, -base.x);
    
    for(auto& seg : segments) {
        bool c1 = pointCoveredBySegment(b1, seg.first, seg.second);
        bool c2 = pointCoveredBySegment(b2, seg.first, seg.second);
        bool c3 = pointCoveredBySegment(b3, seg.first, seg.second);
        bool c4 = pointCoveredBySegment(b4, seg.first, seg.second);
        if(c1 && c2 && c3 && c4) return true;
    }
    return false;
}

int main(){
    int n;
    cin >> n;
    vector<pair<Point,Point>> segments(n);
    
    for(int i = 0; i < n; i++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        segments[i] = {Point(x1,y1), Point(x2,y2)};
    }
    
    double totalArea = 0;
    double totalWeight = 0;
    
    for(int i = 0; i < n; i++) {
        Point a = segments[i].first;
        Point b = segments[i].second;
        Point dir = (b - a) * (1.0/100.0);
        
        for(int j = 0; j < 10000; j++) {
            Point p = a + dir * j;
            if(isValidSquare(p, segments)) {
                double area = 2.0 * (p.x*p.x + p.y*p.y);
                totalArea += area;
                totalWeight += 1.0;
            }
        }
    }
    
    cout << fixed << setprecision(10) << totalArea / totalWeight << endl;
    
    return 0;
}
