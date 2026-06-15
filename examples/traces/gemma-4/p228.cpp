#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * Problem Summary:
 * The task is to find the coordinates of all N islands that form a regular N-gon,
 * given the coordinates of the N1-th and N2-th islands.
 * The islands are numbered in clockwise order.
 * 
 * Approach:
 * 1. A regular N-gon can be inscribed in a circle with center C and radius R.
 * 2. The distance between any two vertices P_i and P_j is d = 2R * sin(PI * |i-j| / N).
 * 3. From the given P_N1 and P_N2, we can calculate the distance d and the radius R.
 * 4. The center C lies on the perpendicular bisector of the segment P_N1P_N2.
 * 5. There are two possible centers at distance R from both P_N1 and P_N2.
 * 6. We can determine which center is correct by using the clockwise ordering property.
 *    Specifically, the cross product of vectors CP_N1 and CP_N2 must match the 
 *    expected sign/value derived from the rotation angle between them.
 * 7. Once the center is found, calculate the initial angle theta_1 and 
 *    compute all coordinates using theta_k = theta_1 - 2*PI*(k-1)/N.
 */

typedef long double ld;

struct Point {
    ld x, y;
};

int main() {
    // Optimizing I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, N1, N2;
    // Read the number of islands and the indices of the given islands
    if (!(cin >> N >> N1 >> N2)) return 0;

    ld x1, y1, x2, y2;
    // Read the coordinates of the N1-th and N2-th islands
    if (!(cin >> x1 >> y1 >> x2 >> y2)) return 0;

    const ld PI = acos(-1.0L);
    ld dx = x2 - x1;
    ld dy = y2 - y1;
    ld d = sqrt(dx * dx + dy * dy);

    // The central angle between P_N1 and P_N2 in a regular N-gon.
    // Since they are indexed in clockwise order, the angle from CP_N1 to CP_N2 is 2*PI*(N1-N2)/N.
    // The absolute difference is used to find R via d = 2R * sin(alpha/2).
    ld alpha = 2.0L * PI * abs(N1 - N2) / (ld)N;
    ld R = d / (2.0L * sin(alpha / 2.0L));

    // The midpoint of the segment connecting the two known islands.
    ld mx = (x1 + x2) / 2.0L;
    ld my = (y1 + y2) / 2.0L;

    // The distance from the midpoint to the center of the circumscribed circle.
    ld h = sqrt(max(0.0L, R * R - (d / 2.0L) * (d / 2.0L)));
    
    // Unit perpendicular vector to the segment P_N1P_N2.
    ld ux = -dy / d;
    ld uy = dx / d;

    // The two potential centers of the N-gon.
    Point c1 = {mx + h * ux, my + h * uy};
    Point c2 = {mx - h * ux, my - h * uy};

    // The expected cross product of CP_N1 and CP_N2.
    // Cross product V1 x V2 = |V1||V2| sin(angle_between_them).
    ld target_cp = R * R * sin(2.0L * PI * (ld)(N1 - N2) / (ld)N);

    Point center;
    bool found = false;

    // Check if c1 is the correct center by verifying the cross product.
    ld v1x = x1 - c1.x;
    ld v1y = y1 - c1.y;
    ld v2x = x2 - c1.x;
    ld v2y = y2 - c1.y;
    ld cp1 = v1x * v2y - v1y * v2x;
    
    // Use a tolerance for floating point comparison.
    if (abs(cp1 - target_cp) < 1e-3 || (abs(target_cp) > 1e-9 && abs(cp1 - target_cp) / abs(target_cp) < 1e-12)) {
        center = c1;
        found = true;
    }

    // If c1 wasn't the center, check c2.
    if (!found) {
        ld v1x2 = x1 - c2.x;
        ld v1y2 = y1 - c2.y;
        ld v2x2 = x2 - c2.x;
        ld v2y2 = y2 - c2.y;
        ld cp2 = v1x2 * v2y2 - v1y2 * v2x2;
        if (abs(cp2 - target_cp) < 1e-3 || (abs(target_cp) > 1e-9 && abs(cp2 - target_cp) / abs(target_cp) < 1e-12)) {
            center = c2;
            found = true;
        }
    }

    // Fallback in case of extreme precision issues (though geometry guarantees one works).
    if (!found) center = c1;

    // Determine the angular position of the first island.
    ld theta1 = atan2(y1 - center.y, x1 - center.x);
    
    // Output the coordinates for all N islands in clockwise order.
    cout << fixed << setprecision(6);
    for (int k = 1; k <= N; ++k) {
        // Islands are clockwise, so theta decreases by 2*PI/N for each successive index.
        ld theta_k = theta1 - 2.0L * PI * (ld)(k - 1) / (ld)N;
        ld xk = center.x + R * cos(theta_k);
        ld yk = center.y + R * sin(theta_k);
        
        // Avoid printing negative zero (-0.000000).
        if (abs(xk) < 1e-8) xk = 0.0L;
        if (abs(yk) < 1e-8) yk = 0.0L;
        
        cout << xk << " " << yk << "\n";
    }

    return 0;
}

