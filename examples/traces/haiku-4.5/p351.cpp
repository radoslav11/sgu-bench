#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    Point operator*(double t) const { return Point(x*t, y*t); }
    double dot(const Point& p) const { return x*p.x + y*p.y; }
    double cross(const Point& p) const { return x*p.y - y*p.x; }
    double len() const { return sqrt(x*x + y*y); }
    Point norm() const { double l = len(); return Point(x/l, y/l); }
};

double dist(Point a, Point b) { return (a-b).len(); }

bool segmentCircleIntersect(Point a, Point b, Point c, double r) {
    Point ab = b - a;
    Point ac = c - a;
    double t = ac.dot(ab) / ab.dot(ab);
    t = max(0.0, min(1.0, t));
    Point closest = a + ab * t;
    return dist(closest, c) < r - 1e-9;
}

vector<Point> getTangentPoints(Point p, Point c, double r) {
    Point pc = c - p;
    double d = pc.len();
    double angle = asin(r / d);
    double baseAngle = atan2(pc.y, pc.x);
    
    vector<Point> result;
    for(int i = -1; i <= 1; i += 2) {
        double a = baseAngle + i * angle;
        Point t = c + Point(cos(a + asin(r/d)), sin(a + asin(r/d))) * r;
        Point dir = Point(cos(a), sin(a));
        t = c - dir * r;
        result.push_back(t);
    }
    return result;
}

vector<Point> getTangents(Point p, Point c, double r) {
    Point pc = c - p;
    double d = pc.len();
    vector<Point> res;
    if(d < r - 1e-9) return res;
    
    double angle = acos(r / d);
    double baseAngle = atan2(pc.y, pc.x);
    
    for(int i = -1; i <= 1; i += 2) {
        double a = baseAngle + i * angle;
        Point t = c + Point(cos(a + M_PI/2), sin(a + M_PI/2)) * r;
        res.push_back(t);
    }
    return res;
}

int main(){
    cout << fixed << setprecision(8);
    double xa, ya, xb, yb, xc, yc, rc, d;
    cin >> xa >> ya >> xb >> yb >> xc >> yc >> rc >> d;
    
    Point A(xa, ya), B(xb, yb), C(xc, yc);
    
    if(!segmentCircleIntersect(A, B, C, rc)) {
        cout << "1\n";
        cout << "S " << xa << " " << ya << " " << xb << " " << yb << "\n";
        return 0;
    }
    
    vector<Point> tangentsA = getTangents(A, C, rc);
    vector<Point> tangentsB = getTangents(B, C, rc);
    
    double bestDist = 1e18;
    Point bestT1, bestT2;
    
    for(auto t1 : tangentsA) {
        for(auto t2 : tangentsB) {
            Point c1 = t1 - C, c2 = t2 - C;
            double angle = acos(min(1.0, max(-1.0, c1.dot(c2) / (rc * rc))));
            double pathDist = dist(A, t1) + rc * angle + dist(t2, B);
            if(pathDist < bestDist) {
                bestDist = pathDist;
                bestT1 = t1;
                bestT2 = t2;
            }
        }
    }
    
    cout << "3\n";
    cout << "S " << A.x << " " << A.y << " " << bestT1.x << " " << bestT1.y << "\n";
    cout << "A " << bestT1.x << " " << bestT1.y << " " << bestT2.x << " " << bestT2.y << "\n";
    cout << "S " << bestT2.x << " " << bestT2.y << " " << B.x << " " << B.y << "\n";
    
    return 0;
}
