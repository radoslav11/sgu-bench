#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    Point operator*(double t) const { return Point(x*t, y*t); }
    double cross(const Point& p) const { return x*p.y - y*p.x; }
    double dot(const Point& p) const { return x*p.x + y*p.y; }
    double dist() const { return hypot(x, y); }
};

double getAngle(Point p) {
    return atan2(p.y, p.x);
}

double segmentLength(Point a, Point b) {
    return (a-b).dist();
}

int main(){
    ios::setflush(0);
    cout << fixed << setprecision(10);
    
    int n;
    cin >> n;
    
    vector<pair<Point, Point>> segments;
    vector<Point> candidates;
    
    for(int i = 0; i < n; i++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        Point p1(x1, y1), p2(x2, y2);
        
        // Check if segment passes through origin
        Point v = p2 - p1;
        Point w = p1;
        
        double t = -1;
        if(v.dist() > EPS) {
            double denom = v.dot(v);
            t = -w.dot(v) / denom;
        }
        
        if(t > EPS && t < 1-EPS) {
            // Origin is strictly between p1 and p2, split it
            Point mid = p1 + v * t;
            segments.push_back({p1, mid});
            segments.push_back({mid, p2});
        } else {
            segments.push_back({p1, p2});
        }
    }
    
    // Try all subsets of segments that form a closed loop
    double maxLen = 0;
    int m = segments.size();
    
    // For small n, we can try to build valid polygons
    // A valid star-like polygon: sort endpoints by angle, connect them properly
    
    for(int mask = 1; mask < (1<<m); mask++) {
        vector<Point> points;
        for(int i = 0; i < m; i++) {
            if(mask & (1<<i)) {
                points.push_back(segments[i].first);
                points.push_back(segments[i].second);
            }
        }
        
        // Remove origin and duplicates
        vector<Point> unique_points;
        for(auto p : points) {
            if(p.dist() > EPS) {
                unique_points.push_back(p);
            }
        }
        
        if(unique_points.size() < 3) continue;
        
        sort(unique_points.begin(), unique_points.end(), [](Point a, Point b) {
            return getAngle(a) < getAngle(b);
        });
        
        // Calculate perimeter
        double len = 0;
        for(int i = 0; i < unique_points.size(); i++) {
            len += segmentLength(unique_points[i], unique_points[(i+1)%unique_points.size()]);
        }
        
        maxLen = max(maxLen, len);
    }
    
    cout << maxLen << "\n";
    
    return 0;
}
