#include<bits/stdc++.h>
using namespace std;

double circularSegmentArea(double r, double d) {
    // Area of circular segment with radius r and distance d from center
    // d should be in range [-r, r]
    if (d >= r) return 0;
    if (d <= -r) return M_PI * r * r;
    
    double angle = acos(d / r);
    return r * r * angle - d * sqrt(r * r - d * d);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long r;
    cin >> r;
    
    int n;
    cin >> n;
    vector<pair<double,double>> java(n);
    for(int i = 0; i < n; i++){
        cin >> java[i].first >> java[i].second;
    }
    
    int m;
    cin >> m;
    vector<pair<double,double>> seeplusplus(m);
    for(int i = 0; i < m; i++){
        cin >> seeplusplus[i].first >> seeplusplus[i].second;
    }
    
    double minArea = M_PI * r * r;
    double maxArea = 0;
    
    // Try many directions
    for(int angle_idx = 0; angle_idx < 10000; angle_idx++){
        double angle = 2 * M_PI * angle_idx / 10000.0;
        double nx = cos(angle);
        double ny = sin(angle);
        
        // Find distance range where Java is on positive side, Seeplusplus on negative
        double minDist = -r;
        double maxDist = r;
        
        for(auto& p : java){
            double d = nx * p.first + ny * p.second;
            minDist = max(minDist, -d); // Java should be on positive side
        }
        
        for(auto& p : seeplusplus){
            double d = nx * p.first + ny * p.second;
            maxDist = min(maxDist, -d); // Seeplusplus should be on negative side
        }
        
        if(minDist <= maxDist){
            // Valid range found
            double area1 = circularSegmentArea(r, minDist);
            double area2 = circularSegmentArea(r, maxDist);
            
            minArea = min(minArea, min(area1, area2));
            maxArea = max(maxArea, max(area1, area2));
        }
    }
    
    cout << fixed << setprecision(12) << minArea << " " << maxArea << endl;
    
    return 0;
}
