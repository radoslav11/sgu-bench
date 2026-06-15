#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

// Cross product of vectors OA and OB where O is origin
long long cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Check if point P is inside or on the boundary of convex polygon
// Polygon vertices should be in counter-clockwise order
bool pointInConvexPolygon(const vector<Point>& polygon, const Point& P) {
    int n = polygon.size();
    
    // Check if point is on the left side (or on) of all edges
    for (int i = 0; i < n; i++) {
        long long cp = cross(polygon[i], polygon[(i + 1) % n], P);
        if (cp < 0) {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<Point> polygon(N);
    for (int i = 0; i < N; i++) {
        cin >> polygon[i].x >> polygon[i].y;
    }
    
    int hits = 0;
    for (int i = 0; i < M; i++) {
        Point explosion;
        cin >> explosion.x >> explosion.y;
        
        if (pointInConvexPolygon(polygon, explosion)) {
            hits++;
        }
    }
    
    if (hits >= K) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
