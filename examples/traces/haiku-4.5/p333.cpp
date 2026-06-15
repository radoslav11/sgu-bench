#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    double cross(const Point& p) const { return x*p.y - y*p.x; }
};

vector<Point> poly;
int n;

bool pointInConvex(Point p) {
    for(int i = 0; i < n; i++) {
        if((poly[(i+1)%n] - poly[i]).cross(p - poly[i]) < -1e-9) return false;
    }
    return true;
}

bool segmentIntersects(Point p1, Point p2) {
    if(pointInConvex(p1) || pointInConvex(p2)) return true;
    for(int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i+1)%n];
        Point seg = p2 - p1, edge = b - a;
        double d = seg.cross(edge);
        if(abs(d) < 1e-9) continue;
        double t = (a-p1).cross(edge)/d, u = (a-p1).cross(seg)/d;
        if(t > -1e-9 && t < 1+1e-9 && u > -1e-9 && u < 1+1e-9) return true;
    }
    return false;
}

int main(){
    cin >> n;
    poly.resize(n);
    for(int i = 0; i < n; i++) cin >> poly[i].x >> poly[i].y;
    
    int cnt = 0, win = 0;
    for(int x1=0; x1<=100; x1++) 
        for(int y1=0; y1<=100; y1++)
            for(int x2=0; x2<=100; x2++)
                for(int y2=0; y2<=100; y2++) {
                    if(segmentIntersects(Point(x1,y1), Point(x2,y2))) win++;
                    cnt++;
                }
    
    cout << fixed << setprecision(10) << (double)win/cnt << endl;
    return 0;
}
