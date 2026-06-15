#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    Point operator*(double t) const { return Point(x*t, y*t); }
    double cross(const Point& p) const { return x*p.y - y*p.x; }
    double dot(const Point& p) const { return x*p.x + y*p.y; }
    double len() const { return sqrt(x*x + y*y); }
};

const double EPS = 1e-9;

Point rayIntersect(Point ray, Point a, Point b) {
    // Intersect ray from origin through 'ray' with segment ab
    Point ba = b - a;
    double t = ray.cross(a) / ray.cross(ba);
    if (t < -EPS || t > 1+EPS) return Point(1e18, 1e18);
    return a + ba * t;
}

double getAngle(Point p) {
    return atan2(p.y, p.x);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<Point> poly(n);
    double totalLen = 0;
    
    for(int i = 0; i < n; i++) {
        cin >> poly[i].x >> poly[i].y;
        Point next = poly[(i+1)%n];
        totalLen += (next - poly[i]).len();
    }
    
    vector<pair<double,double>> shadedIntervals;
    
    for(int i = 0; i < m; i++) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        Point p1(x1, y1), p2(x2, y2);
        
        // Get angles from origin through shelf endpoints
        double a1 = getAngle(p1);
        double a2 = getAngle(p2);
        
        // Find intersections with polygon
        vector<pair<double, Point>> intersections;
        
        for(int j = 0; j < n; j++) {
            Point a = poly[j];
            Point b = poly[(j+1)%n];
            
            Point inter1 = rayIntersect(p1, a, b);
            Point inter2 = rayIntersect(p2, a, b);
            
            if(inter1.x < 1e17) intersections.push_back({a1, inter1});
            if(inter2.x < 1e17) intersections.push_back({a2, inter2});
        }
        
        if(intersections.size() >= 2) {
            sort(intersections.begin(), intersections.end());
            
            // Calculate arc length on polygon from intersection to intersection
            Point i1 = intersections[0].second;
            Point i2 = intersections[1].second;
            
            double arcLen = 0;
            int startEdge = -1, endEdge = -1;
            double startPos = 0, endPos = 0;
            
            // Find which edges contain the intersections
            for(int j = 0; j < n; j++) {
                Point a = poly[j];
                Point b = poly[(j+1)%n];
                
                for(auto& inter : intersections) {
                    Point p = inter.second;
                    Point ba = b - a;
                    double t = ba.len() > EPS ? (p - a).dot(ba) / ba.dot(ba) : -1;
                    if(t >= -EPS && t <= 1+EPS) {
                        if(startEdge == -1) {
                            startEdge = j;
                            startPos = t;
                        } else {
                            endEdge = j;
                            endPos = t;
                        }
                    }
                }
            }
            
            // Calculate shaded arc length going counterclockwise
            if(startEdge != -1 && endEdge != -1) {
                arcLen = 0;
                if(startEdge == endEdge) {
                    arcLen = abs(endPos - startPos) * (poly[startEdge+1] - poly[startEdge]).len();
                } else {
                    arcLen += (1 - startPos) * (poly[startEdge+1] - poly[startEdge]).len();
                    for(int j = (startEdge+1)%n; j != endEdge; j = (j+1)%n) {
                        arcLen += (poly[(j+1)%n] - poly[j]).len();
                    }
                    arcLen += endPos * (poly[endEdge+1] - poly[endEdge]).len();
                }
                shadedIntervals.push_back({0, arcLen});
            }
        }
    }
    
    double shadedTotal = 0;
    for(auto& interval : shadedIntervals) {
        shadedTotal += interval.second;
    }
    
    cout << fixed << setprecision(10) << shadedTotal << endl;
    
    return 0;
}
