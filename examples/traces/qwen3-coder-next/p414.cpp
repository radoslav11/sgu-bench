#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

const double EPS = 1e-9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<tuple<long long, long long, long long>> circles;
    for (int i = 0; i < n; i++) {
        long long x, y, r;
        cin >> x >> y >> r;
        circles.emplace_back(x, y, r);
    }
    
    if (n <= 2) {
        cout << -2 << "\n";
        return 0;
    }
    
    // Try to find 3 non-collinear centers
    int idx1 = 0, idx2 = 1, idx3 = 2;
    bool found_triplet = false;
    
    for (int i = 2; i < n && !found_triplet; i++) {
        idx3 = i;
        long long x1 = get<0>(circles[0]), y1 = get<1>(circles[0]);
        long long x2 = get<0>(circles[1]), y2 = get<1>(circles[1]);
        long long x3 = get<0>(circles[idx3]), y3 = get<1>(circles[idx3]);
        
        // Check if centers are collinear: (x2-x1)*(y3-y1) - (x3-x1)*(y2-y1) != 0
        long long area2 = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        if (area2 != 0) {
            found_triplet = true;
        }
    }
    
    // If no non-collinear triplet found in first n circles, all centers collinear
    if (!found_triplet) {
        // Check for collinear centers case
        // All centers lie on a line. The system has rank <= 2.
        // Check if there's a solution by verifying consistency
        double x1 = get<0>(circles[0]), y1 = get<1>(circles[0]);
        double r1 = get<2>(circles[0]);
        
        // Try to find a potential solution
        // For circles on a line, we can project the problem to 1D
        // But simpler: check if all equations are consistent
        
        // Equation for circle i: 2x_i*x + 2y_i*y - A = x_i^2 + y_i^2 - r_i^2
        
        // Since centers are collinear, we can rotate coordinates to make them collinear on x-axis
        // For simplicity, let's just check consistency using Gaussian elimination on first 3
        
        vector<vector<double>> mat(3, vector<double>(4));
        for (int i = 0; i < 3; i++) {
            double xi = get<0>(circles[i]), yi = get<1>(circles[i]), ri = get<2>(circles[i]);
            mat[i][0] = 2.0 * xi;
            mat[i][1] = 2.0 * yi;
            mat[i][2] = -1.0;
            mat[i][3] = xi * xi + yi * yi - ri * ri;
        }
        
        // Gaussian elimination to check rank
        int rank = 0;
        for (int col = 0; col < 3 && rank < 3; col++) {
            int pivot = rank;
            for (int i = rank + 1; i < 3; i++) {
                if (fabs(mat[i][col]) > fabs(mat[pivot][col])) pivot = i;
            }
            if (fabs(mat[pivot][col]) < EPS) continue;
            
            swap(mat[rank], mat[pivot]);
            double div = mat[rank][col];
            for (int j = col; j < 4; j++) mat[rank][j] /= div;
            
            for (int i = 0; i < 3; i++) {
                if (i != rank && fabs(mat[i][col]) > EPS) {
                    double factor = mat[i][col];
                    for (int j = col; j < 4; j++) mat[i][j] -= factor * mat[rank][j];
                }
            }
            rank++;
        }
        
        bool consistent = true;
        for (int i = 0; i < 3; i++) {
            if (fabs(mat[i][0]) < EPS && fabs(mat[i][1]) < EPS && fabs(mat[i][2]) < EPS) {
                if (fabs(mat[i][3]) > EPS) {
                    consistent = false;
                    break;
                }
            }
        }
        
        if (!consistent) {
            cout << -1 << "\n";
            return 0;
        }
        
        // Since rank < 3 (collinear centers), infinitely many solutions
        cout << -2 << "\n";
        return 0;
    }
    
    // Use the first 3 non-collinear circles
    vector<vector<double>> eqs(3, vector<double>(4));
    vector<int> indices = {0, 1, idx3};
    
    for (int i = 0; i < 3; i++) {
        int idx = indices[i];
        double xi = get<0>(circles[idx]), yi = get<1>(circles[idx]), ri = get<2>(circles[idx]);
        eqs[i][0] = 2.0 * xi;
        eqs[i][1] = 2.0 * yi;
        eqs[i][2] = -1.0;
        eqs[i][3] = xi * xi + yi * yi - ri * ri;
    }
    
    // Compute determinant
    double det = eqs[0][0] * (eqs[1][1] * eqs[2][2] - eqs[1][2] * eqs[2][1])
               - eqs[0][1] * (eqs[1][0] * eqs[2][2] - eqs[1][2] * eqs[2][0])
               + eqs[0][2] * (eqs[1][0] * eqs[2][1] - eqs[1][1] * eqs[2][0]);
    
    if (fabs(det) < EPS) {
        cout << -2 << "\n";
        return 0;
    }
    
    // Cramer's rule
    double dx = eqs[0][3] * (eqs[1][1] * eqs[2][2] - eqs[1][2] * eqs[2][1])
              - eqs[0][1] * (eqs[1][3] * eqs[2][2] - eqs[1][2] * eqs[2][3])
              + eqs[0][2] * (eqs[1][3] * eqs[2][1] - eqs[1][1] * eqs[2][3]);
    
    double dy = eqs[0][0] * (eqs[1][3] * eqs[2][2] - eqs[1][2] * eqs[2][3])
              - eqs[0][3] * (eqs[1][0] * eqs[2][2] - eqs[1][2] * eqs[2][0])
              + eqs[0][2] * (eqs[1][0] * eqs[2][3] - eqs[1][3] * eqs[2][0]);
    
    double dA = eqs[0][0] * (eqs[1][1] * eqs[2][3] - eqs[1][3] * eqs[2][1])
              - eqs[0][1] * (eqs[1][0] * eqs[2][3] - eqs[1][3] * eqs[2][0])
              + eqs[0][3] * (eqs[1][0] * eqs[2][1] - eqs[1][1] * eqs[2][0]);
    
    double x = dx / det;
    double y = dy / det;
    double A = dA / det;
    
    // Verify against all circles
    bool valid = true;
    for (int i = 0; i < n; i++) {
        double xi = get<0>(circles[i]), yi = get<1>(circles[i]), ri = get<2>(circles[i]);
        double residual = 2.0 * xi * x + 2.0 * yi * y - A - (xi * xi + yi * yi - ri * ri);
        if (fabs(residual) > 1e-6) {
            valid = false;
            break;
        }
    }
    
    if (!valid) {
        cout << -1 << "\n";
        return 0;
    }
    
    double r_squared = x * x + y * y - A;
    if (r_squared < -1e-8) {
        cout << -1 << "\n";
        return 0;
    }
    
    double r = sqrt(max(0.0, r_squared));
    
    cout << fixed << setprecision(10);
    cout << x << " " << y << " " << r << "\n";
    
    return 0;
}
