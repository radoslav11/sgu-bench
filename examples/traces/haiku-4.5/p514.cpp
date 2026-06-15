#include <bits/stdc++.h>
using namespace std;

double triangleArea(double a, double b, double c) {
    double s = (a + b + c) / 2;
    double area_sq = s * (s - a) * (s - b) * (s - c);
    if (area_sq < 0) return 0;
    return sqrt(area_sq);
}

double quadrilateralArea(vector<double>& sides, double diagonal) {
    // Split quadrilateral into two triangles using diagonal
    double a = sides[0], b = sides[1], c = sides[2], d = sides[3];
    double area1 = triangleArea(a, b, diagonal);
    double area2 = triangleArea(c, d, diagonal);
    return area1 + area2;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<double> sides(n);
    for(int i = 0; i < n; i++) {
        cin >> sides[i];
    }
    
    if(n == 3) {
        double area = triangleArea(sides[0], sides[1], sides[2]);
        cout << fixed << setprecision(10) << area << endl;
    } else {
        // For quadrilateral and higher, find minimum area
        double minArea = 1e18;
        
        // Try different diagonal lengths for quadrilateral
        if(n == 4) {
            double a = sides[0], b = sides[1], c = sides[2], d = sides[3];
            double minDiag = max(abs(a-b), abs(c-d));
            double maxDiag = min(a+b, c+d);
            
            for(double diag = minDiag; diag <= maxDiag; diag += 0.001) {
                minArea = min(minArea, quadrilateralArea(sides, diag));
            }
        } else {
            minArea = 0;
        }
        
        cout << fixed << setprecision(10) << minArea << endl;
    }
    
    return 0;
}
