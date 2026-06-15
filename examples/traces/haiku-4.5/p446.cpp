#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }
    double dot(const Point& p) const {
        return x * p.x + y * p.y;
    }
    double cross(const Point& p) const {
        return x * p.y - y * p.x;
    }
    double norm() const {
        return sqrt(x * x + y * y);
    }
};

double getAngle(Point v) {
    return atan2(v.y, v.x);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Point> photo1(n), photo2(n);
    
    for(int i = 0; i < n; i++){
        cin >> photo1[i].x >> photo1[i].y;
    }
    
    for(int i = 0; i < n; i++){
        cin >> photo2[i].x >> photo2[i].y;
    }
    
    double bestAngle = PI;
    
    // Try each point in photo2 as correspondence for point 0 in photo1
    for(int j0 = 0; j0 < n; j0++){
        // Try each point in photo2 as correspondence for point 1 in photo1
        for(int j1 = 0; j1 < n; j1++){
            if(j0 == j1) continue;
            
            Point v1 = photo1[1] - photo1[0];
            Point v2 = photo2[j1] - photo2[j0];
            
            double angle1 = getAngle(v1);
            double angle2 = getAngle(v2);
            double rotation = angle2 - angle1;
            
            // Normalize to [0, 2π)
            while(rotation < 0) rotation += 2 * PI;
            while(rotation >= 2 * PI) rotation -= 2 * PI;
            
            // Convert to [0, π]
            if(rotation > PI) rotation = 2 * PI - rotation;
            
            // Verify this rotation works for all points
            bool valid = true;
            for(int i = 0; i < n && valid; i++){
                bool found = false;
                for(int j = 0; j < n; j++){
                    Point vi = photo1[i] - photo1[0];
                    Point vj = photo2[j] - photo2[j0];
                    
                    double a1 = getAngle(vi);
                    double a2 = getAngle(vj);
                    double rot = a2 - a1;
                    while(rot < 0) rot += 2 * PI;
                    while(rot >= 2 * PI) rot -= 2 * PI;
                    if(rot > PI) rot = 2 * PI - rot;
                    
                    if(abs(rot - rotation) < EPS && abs(vi.norm() - vj.norm()) < EPS){
                        found = true;
                        break;
                    }
                }
                if(!found) valid = false;
            }
            
            if(valid){
                bestAngle = min(bestAngle, rotation);
            }
        }
    }
    
    cout << fixed << setprecision(10) << bestAngle << endl;
    
    return 0;
}
