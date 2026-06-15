#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

/**
 * Problem: Height, Bisector and Median (244)
 * Given lengths of height (h), bisector (w), and median (m) from vertex A to side BC.
 * Find coordinates of A, B, and C.
 * 
 * Mathematical analysis:
 * Let side BC lie on the x-axis. Let A be at (0, h).
 * Let B = (xB, 0) and C = (xC, 0).
 * Let M be the midpoint of BC: M = ((xB + xC) / 2, 0).
 * Given median length m: |AM| = sqrt(xM^2 + h^2) = m => xM = sqrt(m^2 - h^2).
 * Let D be the bisector's intersection with BC: D = (xD, 0).
 * Given bisector length w: |AD| = sqrt(xD^2 + h^2) = w => xD = sqrt(w^2 - h^2).
 * 
 * Let xB = xM + delta and xC = xM - delta. Then xM = (xB + xC) / 2.
 * The bisector divides side BC in ratio c:b, where c = AB and b = AC.
 * xD = (b * xB + c * xC) / (b + c), where b = sqrt(xC^2 + h^2) and c = sqrt(xB^2 + h^2).
 * Let g(delta) = (b * (xM + delta) + c * (xM - delta)) / (b + c).
 * g(delta) is a monotonic decreasing function from g(0) = xM to g(inf) = 0 as delta -> inf.
 * For h < w < m, we have 0 < xD < xM, so there is a unique delta > 0 such that g(delta) = xD.
 * Special cases:
 * - h = w = m: Isosceles triangle, any delta > 0 works.
 * - h = w < m: xD = 0, but g(delta) > 0 for all finite delta, so no solution.
 * - h < w = m: xD = xM, but g(delta) = xM only at delta = 0, which is a degenerate triangle.
 */

using namespace std;

double h_val, w_val, m_val;

// Function to calculate the x-coordinate of the bisector given delta
double g(double delta, double xM, double h) {
    double xB = xM + delta;
    double xC = xM - delta;
    double b = sqrt(xC * xC + h * h);
    double c = sqrt(xB * xB + h * h);
    // In case b+c is zero (should not happen since h > 0)
    if (b + c < 1e-15) return xM;
    return (b * xB + c * xC) / (b + c);
}

int main() {
    // Use double for all calculations
    if (!(cin >> h_val >> w_val >> m_val)) return 0;

    // Basic condition for triangle properties from a single vertex
    if (w_val < h_val - 1e-9 || m_val < w_val - 1e-9) {
        cout << "NO" << endl;
        return 0;
    }

    // Check for the equilateral-like case where height = bisector = median
    if (abs(h_val - w_val) < 1e-9 && abs(w_val - m_val) < 1e-9) {
        cout << "YES" << endl;
        cout << fixed << setprecision(8) << 0.0 << " " << h_val << endl;
        cout << fixed << setprecision(8) << -0.5 << " " << 0.0 << endl;
        cout << fixed << setprecision(8) << 0.5 << " " << 0.0 << endl;
    } 
    // Case h = w < m: Bisector is height, but median is longer, impossible for finite triangle
    else if (abs(h_val - w_val) < 1e-9) {
        cout << "NO" << endl;
    } 
    // Case h < w = m: Bisector is median, but height is shorter, only happens if isosceles (delta=0)
    else if (abs(w_val - m_val) < 1e-9) {
        cout << "NO" << endl;
    } 
    // Case h < w < m: Standard triangle solution exists
    else {
        double xM = sqrt(m_val * m_val - h_val * h_val);
        double xD = sqrt(w_val * w_val - h_val * h_val);

        // Binary search for delta in range [0, 10000]
        // Since g(delta) is monotonic decreasing from xM to 0, we find delta such that g(delta) = xD
        double low = 0, high = 10000.0;
        for (int i = 0; i < 100; ++i) {
            double mid = (low + high) / 2.0;
            if (g(mid, xM, h_val) > xD) {
                low = mid;
            } else {
                high = mid;
            }
        }

        double delta = (low + high) / 2.0;
        double xB = xM + delta;
        double xC = xM - delta;

        // Final verification of the computed lengths to ensure accuracy
        double b = sqrt(xC * xC + h_val * h_val);
        double c = sqrt(xB * xB + h_val * h_val);
        double calc_xD = (b * xB + c * xC) / (b + c);
        
        if (abs(calc_xD - xD) < 1e-6) {
            cout << "YES" << endl;
            cout << fixed << setprecision(8) << 0.0 << " " << h_val << endl;
            cout << fixed << setprecision(8) << xB << " " << 0.0 << endl;
            cout << fixed << setprecision(8) << xC << " " << 0.0 << endl;
        } else {
            cout << "NO" << endl;
        }
    }

    return 0;
}

