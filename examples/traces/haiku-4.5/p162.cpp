#include <bits/stdc++.h>
using namespace std;

double det5x5(double m[5][5]) {
    // Compute determinant of 5x5 matrix using cofactor expansion
    double result = 0;
    
    // Expanding along first row
    for (int i = 0; i < 5; i++) {
        // Create 4x4 submatrix
        double sub[4][4];
        for (int row = 1; row < 5; row++) {
            int sub_col = 0;
            for (int col = 0; col < 5; col++) {
                if (col != i) {
                    sub[row-1][sub_col++] = m[row][col];
                }
            }
        }
        
        // Compute 4x4 determinant
        double det4 = 0;
        for (int j = 0; j < 4; j++) {
            double sub3[3][3];
            for (int row = 1; row < 4; row++) {
                int sub_col = 0;
                for (int col = 0; col < 4; col++) {
                    if (col != j) {
                        sub3[row-1][sub_col++] = sub[row][col];
                    }
                }
            }
            
            // Compute 3x3 determinant
            double det3 = sub3[0][0] * (sub3[1][1] * sub3[2][2] - sub3[1][2] * sub3[2][1])
                        - sub3[0][1] * (sub3[1][0] * sub3[2][2] - sub3[1][2] * sub3[2][0])
                        + sub3[0][2] * (sub3[1][0] * sub3[2][1] - sub3[1][1] * sub3[2][0]);
            
            det4 += (j % 2 == 0 ? 1 : -1) * sub[0][j] * det3;
        }
        
        result += (i % 2 == 0 ? 1 : -1) * m[0][i] * det4;
    }
    
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double a, b, c, d, e, f;
    cin >> a >> b >> c >> d >> e >> f;
    
    double m[5][5] = {
        {0, 1, 1, 1, 1},
        {1, 0, a*a, b*b, c*c},
        {1, a*a, 0, d*d, e*e},
        {1, b*b, d*d, 0, f*f},
        {1, c*c, e*e, f*f, 0}
    };
    
    double det = det5x5(m);
    double volume = sqrt(abs(det) / 288.0);
    
    cout << fixed << setprecision(4) << volume << endl;
    
    return 0;
}
