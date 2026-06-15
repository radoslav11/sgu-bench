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
    double len() const { return sqrt(x*x + y*y); }
    double dist(const Point& p) const { return (*this - p).len(); }
};

double polygonArea(vector<Point>& pts) {
    double area = 0;
    int n = pts.size();
    for(int i = 0; i < n; i++) {
        area += pts[i].cross(pts[(i+1)%n]);
    }
    return abs(area) / 2.0;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    double R;
    cin >> N >> R;
    
    vector<array<double,3>> lines(N);
    for(int i = 0; i < N; i++) {
        cin >> lines[i][0] >> lines[i][1] >> lines[i][2];
    }
    
    // Generate many points on circle and find regions
    vector<Point> circlePoints;
    int samples = 10000;
    for(int i = 0; i < samples; i++) {
        double angle = 2 * PI * i / samples;
        circlePoints.push_back(Point(R * cos(angle), R * sin(angle)));
    }
    
    // For each arc on circle, determine which side of each line it's on
    vector<vector<int>> regions;
    vector<double> regionAreas;
    
    // Brute force: sample points and group them
    set<vector<int>> signatureSet;
    map<vector<int>, vector<Point>> regionPoints;
    
    for(auto& pt : circlePoints) {
        vector<int> sig;
        for(int i = 0; i < N; i++) {
            double val = lines[i][0] * pt.x + lines[i][1] * pt.y + lines[i][2];
            sig.push_back(val > EPS ? 1 : (val < -EPS ? -1 : 0));
        }
        regionPoints[sig].push_back(pt);
    }
    
    for(auto& [sig, pts] : regionPoints) {
        if(pts.empty()) continue;
        
        // Sort points by angle
        Point center;
        for(auto& p : pts) center = center + p;
        center = center * (1.0 / pts.size());
        
        sort(pts.begin(), pts.end(), [&](Point a, Point b) {
            return atan2(a.y - center.y, a.x - center.x) < 
                   atan2(b.y - center.y, b.x - center.x);
        });
        
        double area = polygonArea(pts);
        if(area > EPS) {
            regionAreas.push_back(area);
        }
    }
    
    // Sort areas in descending order
    sort(regionAreas.begin(), regionAreas.end(), greater<double>());
    
    double carlsson = 0, winnie = 0;
    for(int i = 0; i < regionAreas.size(); i++) {
        if(i % 2 == 0) {
            carlsson += regionAreas[i];
        } else {
            winnie += regionAreas[i];
        }
    }
    
    cout << fixed << setprecision(10) << carlsson << " " << winnie << endl;
    
    return 0;
}
