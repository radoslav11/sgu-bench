#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

struct Triangle {
    Point p[3];
};

bool isRightIsosceles(Triangle t) {
    // Check if triangle is right isosceles with legs parallel to axes
    vector<double> dists;
    for(int i = 0; i < 3; i++) {
        for(int j = i+1; j < 3; j++) {
            double dx = t.p[i].x - t.p[j].x;
            double dy = t.p[i].y - t.p[j].y;
            dists.push_back(dx*dx + dy*dy);
        }
    }
    sort(dists.begin(), dists.end());
    
    // Should be isosceles right: a^2 + a^2 = c^2
    if(fabs(dists[0] - dists[1]) > 1e-9) return false;
    if(fabs(dists[0] + dists[1] - dists[2]) > 1e-9) return false;
    
    // Check if legs are parallel to axes
    int axisParallel = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = i+1; j < 3; j++) {
            if(fabs(t.p[i].x - t.p[j].x) < 1e-9 || fabs(t.p[i].y - t.p[j].y) < 1e-9) {
                axisParallel++;
            }
        }
    }
    return axisParallel == 2;
}

bool isInside(Point p, double n) {
    return p.x >= -1e-9 && p.y >= -1e-9 && p.x + p.y <= n + 1e-9;
}

bool triangleInside(Triangle t, double n) {
    return isInside(t.p[0], n) && isInside(t.p[1], n) && isInside(t.p[2], n);
}

double triangleArea(Triangle t) {
    return fabs((t.p[0].x*(t.p[1].y - t.p[2].y) + 
                 t.p[1].x*(t.p[2].y - t.p[0].y) + 
                 t.p[2].x*(t.p[0].y - t.p[1].y)) / 2.0);
}

bool segmentsIntersect(Point a, Point b, Point c, Point d) {
    auto ccw = [](Point A, Point B, Point C) {
        return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
    };
    return ccw(a,c,d) != ccw(b,c,d) && ccw(a,b,c) != ccw(a,b,d);
}

bool trianglesOverlap(Triangle t1, Triangle t2) {
    // Check if segments intersect
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(segmentsIntersect(t1.p[i], t1.p[(i+1)%3], t2.p[j], t2.p[(j+1)%3])) {
                return true;
            }
        }
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while(t--) {
        int n, m;
        cin >> n >> m;
        
        vector<Triangle> triangles(m);
        double totalArea = 0;
        bool valid = true;
        
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < 3; j++) {
                cin >> triangles[i].p[j].x >> triangles[i].p[j].y;
            }
            
            if(!isRightIsosceles(triangles[i]) || !triangleInside(triangles[i], n)) {
                valid = false;
            }
            totalArea += triangleArea(triangles[i]);
        }
        
        for(int i = 0; i < m && valid; i++) {
            for(int j = i+1; j < m; j++) {
                if(trianglesOverlap(triangles[i], triangles[j])) {
                    valid = false;
                    break;
                }
            }
        }
        
        double expectedArea = n * n / 2.0;
        if(fabs(totalArea - expectedArea) > 1e-6) {
            valid = false;
        }
        
        cout << (valid ? "YES" : "NO") << "\n";
    }
    
    return 0;
}
