#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

double determinant2x2(double a, double b, double c, double d) {
    return a * d - b * c;
}

double determinant3x3(const vector<vector<double>>& m) {
    double det = 0;
    for (int j = 0; j < 3; j++) {
        double minor = determinant2x2(m[1][(j+1)%3], m[1][(j+2)%3],
                                      m[2][(j+1)%3], m[2][(j+2)%3]);
        det += m[0][j] * (j % 2 == 0 ? 1 : -1) * minor;
    }
    return det;
}

double determinant4x4(const vector<vector<double>>& m) {
    double det = 0;
    for (int j = 0; j < 4; j++) {
        vector<vector<double>> minor(3, vector<double>(3));
        for (int i = 1; i < 4; i++) {
            int colIdx = 0;
            for (int k = 0; k < 4; k++) {
                if (k != j) {
                    minor[i-1][colIdx++] = m[i][k];
                }
            }
        }
        double sign = (j % 2 == 0) ? 1 : -1;
        det += sign * m[0][j] * determinant3x3(minor);
    }
    return det;
}

double determinant5x5(const vector<vector<double>>& m) {
    double det = 0;
    for (int j = 0; j < 5; j++) {
        vector<vector<double>> minor(4, vector<double>(4));
        for (int i = 1; i < 5; i++) {
            int colIdx = 0;
            for (int k = 0; k < 5; k++) {
                if (k != j) {
                    minor[i-1][colIdx++] = m[i][k];
                }
            }
        }
        double sign = (j % 2 == 0) ? 1 : -1;
        det += sign * m[0][j] * determinant4x4(minor);
    }
    return det;
}

int main() {
    double AB, AC, AD, BC, BD, CD;
    cin >> AB >> AC >> AD >> BC >> BD >> CD;
    
    double AB2 = AB * AB;
    double AC2 = AC * AC;
    double AD2 = AD * AD;
    double BC2 = BC * BC;
    double BD2 = BD * BD;
    double CD2 = CD * CD;
    
    // Build Cayley-Menger matrix (5x5)
    vector<vector<double>> CM(5, vector<double>(5, 0.0));
    
    // Row 0: [0, 1, 1, 1, 1]
    CM[0][0] = 0;
    for (int j = 1; j < 5; j++) {
        CM[0][j] = 1;
    }
    
    // Row 1: [1, 0, AB^2, AC^2, AD^2]
    CM[1][0] = 1;
    CM[1][1] = 0;
    CM[1][2] = AB2;
    CM[1][3] = AC2;
    CM[1][4] = AD2;
    
    // Row 2: [1, AB^2, 0, BC^2, BD^2]
    CM[2][0] = 1;
    CM[2][1] = AB2;
    CM[2][2] = 0;
    CM[2][3] = BC2;
    CM[2][4] = BD2;
    
    // Row 3: [1, AC^2, BC^2, 0, CD^2]
    CM[3][0] = 1;
    CM[3][1] = AC2;
    CM[3][2] = BC2;
    CM[3][3] = 0;
    CM[3][4] = CD2;
    
    // Row 4: [1, AD^2, BD^2, CD^2, 0]
    CM[4][0] = 1;
    CM[4][1] = AD2;
    CM[4][2] = BD2;
    CM[4][3] = CD2;
    CM[4][4] = 0;
    
    double det = determinant5x5(CM);
    
    // Volume^2 = det / 288
    if (det < 0) {
        // In case of floating point issues, clamp to 0
        det = 0;
    }
    
    double volume_squared = det / 288.0;
    double volume = sqrt(volume_squared);
    
    cout << fixed << setprecision(4) << volume << endl;
    
    return 0;
}
