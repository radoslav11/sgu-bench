#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

struct Point {
    long long x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

long long cross_product(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convex_hull(vector<Point>& P) {
    int n = P.size();
    if (n <= 2) return P;
    vector<Point> H(2 * n);
    sort(P.begin(), P.end());
    int k = 0;
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross_product(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross_product(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }
    H.resize(k - 1);
    return H;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;
    
    vector<Point> P(n);
    for (int i = 0; i < n; ++i) {
        cin >> P[i].x >> P[i].y;
    }
    
    vector<Point> hull = convex_hull(P);
    
    int m;
    if (!(cin >> m)) return 0;
    
    for (int j = 0; j < m; ++j) {
        Point A, B;
        cin >> A.x >> A.y >> B.x >> B.y;
        
        long long dx = B.x - A.x;
        long long dy = B.y - A.y;
        double L = sqrt(double(dx) * dx + double(dy) * dy);
        
        if (L == 0.0) {
            cout << "0.00\n";
            continue;
        }
        
        double t_min = 0.0;
        double t_max = 1.0;
        int h_n = hull.size();
        
        for (int i = 0; i < h_n; ++i) {
            Point P1 = hull[i];
            Point P2 = hull[(i + 1) % h_n];
            
            long long nx = -(P2.y - P1.y);
            long long ny = (P2.x - P1.x);
            
            long long c0 = nx * (A.x - P1.x) + ny * (A.y - P1.y);
            long long c1 = nx * dx + ny * dy;
            
            if (c1 == 0) {
                if (c0 <= 0) {
                    t_max = -1.0;
                    break;
                }
            } else if (c1 > 0) {
                double t0 = -(double)c0 / c1;
                if (t0 > t_min) t_min = t0;
            } else { // c1 < 0
                double t0 = -(double)c0 / c1;
                if (t0 < t_max) t_max = t0;
            }
        }
        
        double len = max(0.0, t_max - t_min) * L;
        cout << fixed << setprecision(2) << len << "\n";
    }
    
    return 0;
}
