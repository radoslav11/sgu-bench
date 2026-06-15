#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * The problem asks for the volume of a tetrahedron given its six edge lengths.
 * A common way to calculate the volume of a tetrahedron when only edge lengths
 * are known is by using the Cayley-Menger determinant.
 * 
 * For a tetrahedron with vertices 1, 2, 3, 4 and edge lengths d_ij,
 * the volume V satisfies:
 * 288 * V^2 = det(M)
 * where M is a 5x5 matrix:
 * [ 0      d12^2   d13^2   d14^2   1 ]
 * [ d21^2  0       d23^2   d24^2   1 ]
 * [ d31^2  d32^2   0       d34^2   1 ]
 * [ d41^2  d42^2   d43^2   0       1 ]
 * [ 1      1       1       1       0 ]
 * 
 * Edge indices based on input (AB, AC, AD, BC, BD, CD):
 * d12 = AB
 * d13 = AC
 * d14 = AD
 * d23 = BC
 * d24 = BD
 * d34 = CD
 */

typedef long double ld;

/**
 * Calculates the determinant of an n x n matrix using Gaussian elimination with partial pivoting.
 * 
 * @param mat The matrix for which to find the determinant.
 * @return The determinant of the matrix.
 */
ld determinant(vector<vector<ld>>& mat) {
    int n = mat.size();
    ld det_val = 1.0;
    for (int i = 0; i < n; ++i) {
        int pivot = i;
        for (int j = i + 1; j < n; ++j) {
            if (abs(mat[j][i]) > abs(mat[pivot][i])) {
                pivot = j;
            }
        }
        if (abs(mat[pivot][i]) < 1e-25) return 0.0;
        if (pivot != i) {
            swap(mat[i], mat[pivot]);
            det_val *= -1;
        }
        det_val *= mat[i][i];
        for (int j = i + 1; j < n; ++j) {
            ld factor = mat[j][i] / mat[i][i];
            for (int k = i + 1; k < n; ++k) {
                mat[j][k] -= factor * mat[i][k];
            }
        }
    }
    return det_val;
}

int main() {
    // Speed up standard I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ld ab, ac, ad, bc, bd, cd;
    // Reading six edge lengths as specified: AB, AC, AD, BC, BD, CD
    if (!(cin >> ab >> ac >> ad >> bc >> bd >> cd)) return 0;

    // Building the Cayley-Menger matrix
    vector<vector<ld>> mat(5, vector<ld>(5));
    mat[0][0] = 0; mat[0][1] = ab * ab; mat[0][2] = ac * ac; mat[0][3] = ad * ad; mat[0][4] = 1;
    mat[1][0] = ab * ab; mat[1][1] = 0; mat[1][2] = bc * bc; mat[1][3] = bd * bd; mat[1][4] = 1;
    mat[2][0] = ac * ac; mat[2][1] = bc * bc; mat[2][2] = 0; mat[2][3] = cd * cd; mat[2][4] = 1;
    mat[3][0] = ad * ad; mat[3][1] = bd * bd; mat[3][2] = cd * cd; mat[3][3] = 0; mat[3][4] = 1;
    mat[4][0] = 1; mat[4][1] = 1; mat[4][2] = 1; mat[4][3] = 1; mat[4][4] = 0;

    // Calculate the determinant of the matrix
    ld det = determinant(mat);
    
    // Applying the 288 * V^2 formula
    ld v2 = det / 288.0;
    // The determinant should be positive for a valid tetrahedron.
    // If precision errors lead to a tiny negative number, we correct it.
    if (v2 < 0) v2 = 0;
    ld v = sqrt(v2);

    // Outputting the result with exactly 4 digits after the decimal point
    cout << fixed << setprecision(4) << (double)v << endl;

    return 0;
}

