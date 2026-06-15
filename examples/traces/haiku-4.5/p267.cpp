#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

double polygonArea(vector<Point>& poly) {
    double area = 0;
    int n = poly.size();
    for(int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += poly[i].x * poly[j].y;
        area -= poly[j].x * poly[i].y;
    }
    return abs(area) / 2.0;
}

bool canSeparate(Point c1, Point c2, Point center) {
    // Check if center lies on the line segment between c1 and c2
    double d1 = hypot(c1.x - center.x, c1.y - center.y);
    double d2 = hypot(c2.x - center.x, c2.y - center.y);
    double d12 = hypot(c1.x - c2.x, c1.y - c2.y);
    
    // If center is on segment, they can't be separated by a line through center
    if(abs(d1 + d2 - d12) < 1e-9) return false;
    
    // Otherwise, a line through center can always separate them
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    vector<double> validAreas;
    
    for(int i = 0; i < N; i++) {
        vector<Point> corners(4);
        Point c1, c2;
        
        for(int j = 0; j < 4; j++) {
            cin >> corners[j].x >> corners[j].y;
        }
        cin >> c1.x >> c1.y >> c2.x >> c2.y;
        
        // Calculate center of rectangle
        Point center;
        center.x = (corners[0].x + corners[2].x) / 2.0;
        center.y = (corners[0].y + corners[2].y) / 2.0;
        
        // Check if candles can be separated
        if(canSeparate(c1, c2, center)) {
            double area = polygonArea(corners);
            validAreas.push_back(area);
        }
    }
    
    if(validAreas.size() < K) {
        cout << fixed << setprecision(3) << "0.000 0.000\n";
        return 0;
    }
    
    sort(validAreas.begin(), validAreas.end());
    
    // Pessimist: sum of K smallest
    double pessimist = 0;
    for(int i = 0; i < K; i++) {
        pessimist += validAreas[i];
    }
    
    // Optimist: sum of K largest
    double optimist = 0;
    for(int i = validAreas.size() - K; i < validAreas.size(); i++) {
        optimist += validAreas[i];
    }
    
    cout << fixed << setprecision(3) << pessimist << " " << optimist << "\n";
    
    return 0;
}
