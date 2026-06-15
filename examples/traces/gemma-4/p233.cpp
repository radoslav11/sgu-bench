#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * The problem asks to find a point C on a circle such that the angle ACB is maximized,
 * where A and B are two points strictly inside the circle.
 * 
 * A key geometric property is that the angle ACB is maximized when the circle
 * passing through A, B, and C is tangent to the given circle at point C.
 * 
 * Let the circle center be O(X0, Y0) and radius R.
 * By translating the coordinate system so that O is at (0, 0), we have:
 * A = (Xa, Ya), B = (Xb, Yb), and the circle is x^2 + y^2 = R^2.
 * 
 * The perpendicular bisector of AB is given by:
 * 2x(Xb - Xa) + 2y(Yb - Ya) = Xb^2 + Yb^2 - Xa^2 - Ya^2
 * Let this be ax + by = c, where:
 * a = 2(Xb - Xa)
 * b = 2(Yb - Ya)
 * c = Xb^2 + Yb^2 - Xa^2 - Ya^2
 * 
 * For the circle through A, B, and C to be tangent to the circle at C,
 * the center of this circle, Ok, must lie on the perpendicular bisector of AB 
 * and on the radius OC. Thus, Ok = (k * cos(theta), k * sin(theta)) for some k,
 * where C = (R * cos(theta), R * sin(theta)).
 * 
 * From Ok being on the line ax + by = c:
 * k * (a * cos(theta) + b * sin(theta)) = c  => k = c / (a * cos(theta) + b * sin(theta))
 * 
 * From Ok being the center of a circle passing through A and C:
 * distance(Ok, A)^2 = distance(Ok, C)^2
 * (k * cos(theta) - Xa)^2 + (k * sin(theta) - Ya)^2 = (k - R)^2
 * k^2 - 2k(Xa * cos(theta) + Ya * sin(theta)) + Xa^2 + Ya^2 = k^2 - 2kR + R^2
 * 2k(R - (Xa * cos(theta) + Ya * sin(theta))) = R^2 - Xa^2 - Ya^2
 * 
 * Let D = R^2 - Xa^2 - Ya^2.
 * 2 * [c / (a * cos(theta) + b * sin(theta))] * (R - Xa * cos(theta) - Ya * sin(theta)) = D
 * 2c * (R - Xa * cos(theta) - Ya * sin(theta)) = D * (a * cos(theta) + b * sin(theta))
 * 2cR - 2c * Xa * cos(theta) - 2c * Ya * sin(theta) = Da * cos(theta) + Db * sin(theta)
 * (Da + 2c * Xa) * cos(theta) + (Db + 2c * Ya) * sin(theta) = 2cR
 * 
 * This is an equation of the form E * cos(theta) + F * sin(theta) = G.
 * We can solve this for theta using:
 * theta = atan2(F, E) +/- acos(G / sqrt(E^2 + F^2))
 */

typedef long double ld;

struct Point {
    ld x, y;
};

ld calculate_angle(Point a, Point b, Point c) {
    ld v1x = a.x - c.x;
    ld v1y = a.y - c.y;
    ld v2x = b.x - c.x;
    ld v2y = b.y - c.y;
    ld dot = v1x * v2x + v1y * v2y;
    ld mag1 = sqrt(v1x * v1x + v1y * v1y);
    ld mag2 = sqrt(v2x * v2x + v2y * v2y);
    ld cos_val = dot / (mag1 * mag2);
    // Clamp cos_val to [-1, 1] to avoid domain error in acos
    if (cos_val > 1.0L) cos_val = 1.0L;
    if (cos_val < -1.0L) cos_val = -1.0L;
    return acos(cos_val);
}

void solve() {
    ld x0, y0, r, xa, ya, xb, yb;
    if (!(cin >> x0 >> y0 >> r >> xa >> ya >> xb >> yb)) return;

    // Translate everything so the circle center is (0,0)
    xa -= x0; ya -= y0;
    xb -= x0; yb -= y0;

    ld a = 2.0L * (xb - xa);
    ld b = 2.0L * (yb - ya);
    ld c = (xb * xb + yb * yb) - (xa * xa + ya * ya);
    ld D = r * r - (xa * xa + ya * ya);

    ld E = D * a + 2.0L * c * xa;
    ld F = D * b + 2.0L * c * ya;
    ld G = 2.0L * c * r;

    vector<ld> candidates;
    ld M2 = E * E + F * F;
    ld M = sqrt(M2);

    if (M > 1e-15L) {
        ld ratio = G / M;
        if (ratio > 1.0L) ratio = 1.0L;
        if (ratio < -1.0L) ratio = -1.0L;
        ld base_theta = atan2(F, E);
        ld diff = acos(ratio);
        candidates.push_back(base_theta + diff);
        candidates.push_back(base_theta - diff);
    } else {
        // If M is near 0, A and B are likely the same or other degenerate cases.
        // In that case, any point on the circle works.
        candidates.push_back(0.0L);
    }

    // Also check the points where the perpendicular bisector intersects the circle.
    // This covers cases where the tangent condition might not be sufficient.
    // aR * cos(theta) + bR * sin(theta) = c  =>  a * cos(theta) + b * sin(theta) = c/r
    ld c_div_r = c / r;
    ld M_bisect2 = a * a + b * b;
    ld M_bisect = sqrt(M_bisect2);
    if (M_bisect > 1e-15L) {
        ld ratio_bisect = c_div_r / M_bisect;
        if (ratio_bisect > 1.0L) ratio_bisect = 1.0L;
        if (ratio_bisect < -1.0L) ratio_bisect = -1.0L;
        ld base_theta_bisect = atan2(b, a);
        ld diff_bisect = acos(ratio_bisect);
        candidates.push_back(base_theta_bisect + diff_bisect);
        candidates.push_back(base_theta_bisect - diff_bisect);
    }

    Point A = {xa, ya}, B = {xb, yb};
    ld max_angle = -1.0L;
    Point best_C = {0, 0};

    for (ld theta : candidates) {
        Point C = {r * cos(theta), r * sin(theta)};
        ld ang = calculate_angle(A, B, C);
        if (ang > max_angle) {
            max_angle = ang;
            best_C = C;
        }
    }

    // If no point found, just pick an arbitrary point on the circle.
    if (max_angle < -0.5L) {
        best_C = {r, 0};
    }

    cout << fixed << setprecision(6) << (double)(best_C.x + x0) << " " << (double)(best_C.y + y0) << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    if (!(cin >> N)) return 0;
    while (N--) {
        solve();
    }
    return 0;
}

