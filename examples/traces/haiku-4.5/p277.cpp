#include<bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

long long cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convexHull(vector<Point> points) {
    int n = points.size();
    if (n <= 2) return points;
    
    sort(points.begin(), points.end());
    
    vector<Point> hull;
    
    // Build lower hull
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    
    // Build upper hull
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while (hull.size() > lower_size && cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    
    hull.pop_back(); // Remove duplicate point
    return hull;
}

long long doubledArea(const vector<Point>& hull) {
    long long area = 0;
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += hull[i].x * hull[j].y - hull[j].x * hull[i].y;
    }
    return abs(area);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    
    vector<Point> points;
    points.push_back({x1, y1});
    points.push_back({x2, y2});
    points.push_back({x3, y3});
    
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++) {
        long long x, y;
        cin >> x >> y;
        points.push_back({x, y});
        
        vector<Point> hull = convexHull(points);
        long long area = doubledArea(hull);
        cout << area << "\n";
    }
    
    return 0;
}
