#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/**
 * Problem: Construct a triangle ABC given |AB|=c, |AC|=b, and the median |AM|=m.
 * M is the midpoint of BC.
 * 
 * Let A be at the origin (0,0).
 * Let B be at (c, 0).
 * Let C be at (xC, yC).
 * 
 * M is the midpoint of BC, so M = (B + C) / 2 = ((c + xC) / 2, yC / 2).
 * Given |AM| = m, then |AM|^2 = m^2:
 * ((c + xC) / 2)^2 + (yC / 2)^2 = m^2
 * (c^2 + 2*c*xC + xC^2 + yC^2) / 4 = m^2
 * 
 * We know |AC| = b, so xC^2 + yC^2 = b^2.
 * Substituting this into the equation:
 * (c^2 + 2*c*xC + b^2) / 4 = m^2
 * c^2 + 2*c*xC + b^2 = 4 * m^2
 * 2 * c * xC = 4 * m^2 - c^2 - b^2
 * xC = (4 * m^2 - c^2 - b^2) / (2 * c)
 * 
 * Once we have xC, we find yC:
 * yC^2 = b^2 - xC^2
 * yC = sqrt(b^2 - xC^2)
 * 
 * A triangle exists if A, B, and C are not collinear, which means yC != 0.
 * This happens if xC^2 < b^2, or equivalently, |c - b| < 2m < c + b.
 * If yC^2 <= 0, the triangle is degenerate or doesn't exist.
 */

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long double c, b, m;
    // Read input. Using long double for high precision.
    if (!(cin >> c >> b >> m)) return 0;

    // Calculate the x-coordinate of vertex C, given A=(0,0) and B=(c,0)
    long double xC = (4.0L * m * m - c * c - b * b) / (2.0L * c);
    
    // Calculate yC^2 to check if a non-degenerate triangle exists.
    long double yC2 = b * b - xC * xC;

    // A non-degenerate triangle must have a non-zero area.
    // Area = 0.5 * |c * yC|. Thus we need yC != 0.
    // Using a small epsilon to account for floating-point precision.
    if (yC2 <= 1e-11L) {
        cout << "Mission impossible" << endl;
    } else {
        long double yC = sqrtl(yC2);
        
        // Output coordinates of A, B, and C as requested.
        // A = (0.0, 0.0)
        // B = (c, 0.0)
        // C = (xC, yC)
        cout << fixed << setprecision(5) << 0.0L << " " << 0.0L << "\n";
        cout << fixed << setprecision(5) << c << " " << 0.0L << "\n";
        cout << fixed << setprecision(5) << xC << " " << yC << "\n";
    }

    return 0;
}

