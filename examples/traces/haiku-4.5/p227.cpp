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
    double norm() const { return sqrt(x*x + y*y); }
};

struct Circle {
    Point c;
    double r;
};

Circle getCircle(Point a, Point b, Point c) {
    double ax = a.x, ay = a.y;
    double bx = b.x, by = b.y;
    double cx = c.x, cy = c.y;
    
    double d = 2*(ax*(by-cy) + bx*(cy-ay) + cx*(ay-by));
    if(abs(d) < 1e-9) return {Point(0,0), 0};
    
    double ux = ((ax*ax+ay*ay)*(by-cy) + (bx*bx+by*by)*(cy-ay) + (cx*cx+cy*cy)*(ay-by))/d;
    double uy = ((ax*ax+ay*ay)*(cx-bx) + (bx*bx+by*by)*(ax-cx) + (cx*cx+cy*cy)*(bx-ax))/d;
    
    Point center(ux, uy);
    double r = (a-center).norm();
    return {center, r};
}

bool onArc(Point p, Point a, Point b, Point center) {
    double eps = 1e-6;
    double dist = (p-center).norm();
    double r = (a-center).norm();
    if(abs(dist - r) > eps) return false;
    
    Point pa = a - center;
    Point pb = b - center;
    Point pp = p - center;
    
    double angle_p = atan2(pp.y, pp.x);
    double angle_a = atan2(pa.y, pa.x);
    double angle_b = atan2(pb.y, pb.x);
    
    double angle_mid = atan2((pa+pb).y, (pa+pb).x);
    
    while(angle_p < angle_a - M_PI) angle_p += 2*M_PI;
    while(angle_p > angle_a + M_PI) angle_p -= 2*M_PI;
    while(angle_b < angle_a - M_PI) angle_b += 2*M_PI;
    while(angle_b > angle_a + M_PI) angle_b -= 2*M_PI;
    
    return (angle_p >= min(angle_a, angle_b) - eps && angle_p <= max(angle_a, angle_b) + eps);
}

vector<Point> circleIntersect(Circle c1, Circle c2) {
    vector<Point> res;
    double eps = 1e-9;
    double d = (c1.c - c2.c).norm();
    
    if(d < eps) {
        if(abs(c1.r - c2.r) < eps) return res; // infinity
        return res; // no intersection
    }
    
    if(d > c1.r + c2.r + eps || d < abs(c1.r - c2.r) - eps) return res;
    
    double a = (d*d + c1.r*c1.r - c2.r*c2.r) / (2*d);
    Point p2 = c1.c + (c2.c - c1.c) * (a/d);
    
    double h2 = c1.r*c1.r - a*a;
    if(h2 < -eps) return res;
    h2 = max(0.0, h2);
    double h = sqrt(h2);
    
    Point offset = (c2.c - c1.c) * (h/d);
    offset = Point(-offset.y, offset.x);
    
    res.push_back(p2 + offset);
    if(h > eps) res.push_back(p2 - offset);
    
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<array<Point,3>> arcs(n);
    vector<Circle> circles(n);
    
    for(int i=0; i<n; i++) {
        for(int j=0; j<3; j++) {
            cin >> arcs[i][j].x >> arcs[i][j].y;
        }
        circles[i] = getCircle(arcs[i][0], arcs[i][1], arcs[i][2]);
    }
    
    set<pair<double,double>> crossings;
    
    for(int i=0; i<n; i++) {
        for(int j=i+1; j<n; j++) {
            vector<Point> pts = circleIntersect(circles[i], circles[j]);
            for(auto& p : pts) {
                if(onArc(p, arcs[i][0], arcs[i][1], circles[i].c) &&
                   onArc(p, arcs[j][0], arcs[j][1], circles[j].c)) {
                    crossings.insert({round(p.x*1000)/1000.0, round(p.y*1000)/1000.0});
                }
            }
        }
    }
    
    cout << crossings.size() << "\n";
    for(auto& p : crossings) {
        cout << fixed << setprecision(3) << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
