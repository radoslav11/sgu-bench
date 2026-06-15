#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

/**
 * Problem: Archipelago (Regular N-gon)
 * The goal is to find the coordinates of all N islands that form a regular N-gon.
 * The islands are numbered clockwise: N1, N2, ..., NN.
 * Given the coordinates of two islands (N1 and N2), we find the center and radius.
 */

int main() {
    // Optimize standard input/output operations.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, N1, N2;
    // Read N, N1, and N2. N is the number of islands, N1 and N2 are the given indices.
    if (!(cin >> N >> N1 >> N2)) return 0;

    long double xN1, yN1, xN2, yN2;
    // Read the coordinates of islands N1 and N2.
    if (!(cin >> xN1 >> yN1 >> xN2 >> yN2)) return 0;

    // The islands are numbered clockwise, meaning the angle between consecutive vertices is -2*PI/N.
    // Calculate the number of steps (k) to get from N1 to N2 in a clockwise direction.
    int k = (N2 - N1 + N) % N;

    // Using a high-precision value for PI.
    const long double PI = acos(-1.0L);
    
    // Calculate the rotation angle phi for k steps clockwise.
    long double phi = -2.0L * (long double)k * PI / (long double)N;

    long double c = cos(phi);
    long double s = sin(phi);

    /**
     * A vertex P2 is the rotation of vertex P1 around the center C by angle phi.
     * P2 - C = Rot(phi) * (P1 - C)
     * 
     * In coordinate terms:
     * x2 - xC = (x1 - xC) * cos(phi) - (y1 - yC) * sin(phi)
     * y2 - yC = (x1 - xC) * sin(phi) + (y1 - yC) * cos(phi)
     * 
     * Rearranging to form a linear system for xC and yC:
     * (1 - cos(phi)) * xC + sin(phi) * yC = x2 - x1*cos(phi) + y1*sin(phi)
     * -sin(phi) * xC + (1 - cos(phi)) * yC = y2 - x1*sin(phi) - y1*cos(phi)
     * 
     * Let:
     * A = 1 - c
     * B = s
     * C_coeff = -s
     * D = 1 - c
     * E = x2 - x1*c + y1*s
     * F = y2 - x1*s - y1*c
     * 
     * The determinant Det = A*D - B*C_coeff = (1-c)^2 + s^2 = 2 - 2c.
     * Cramer's rule gives:
     * xC = (E*D - B*F) / Det
     * yC = (A*F - E*C_coeff) / Det
     */

    long double E = xN2 - xN1 * c + yN1 * s;
    long double F = yN2 - xN1 * s - yN1 * c;

    long double Det = 2.0L * (1.0L - c);
    long double xC = (E * (1.0L - c) - s * F) / Det;
    long double yC = (F * (1.0L - c) + s * E) / Det;

    // Radius R and the polar angle alpha1 of island N1 from the center C.
    long double R = sqrt((xN1 - xC) * (xN1 - xC) + (yN1 - yC) * (yN1 - yC));
    long double alpha1 = atan2(yN1 - yC, xN1 - xC);

    // Set decimal precision to 6 as required.
    cout << fixed << setprecision(6);
    for (int i = 1; i <= N; ++i) {
        // The angle of the i-th island decreases in steps of 2*PI/N (clockwise movement).
        long double alpha_i = alpha1 - (long double)(i - N1) * 2.0L * PI / (long double)N;
        long double xi = xC + R * cos(alpha_i);
        long double yi = yC + R * sin(alpha_i);
        
        // Clean up extremely small values near zero to avoid "-0.000000" outputs.
        if (fabsl(xi) < 1e-11L) xi = 0.0L;
        if (fabsl(yi) < 1e-11L) yi = 0.0L;
        
        cout << xi << " " << yi << "\n";
    }

    return 0;
}

