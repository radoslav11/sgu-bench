#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

/**
 * Problem: Berlion
 * Task: Find the area of the surface of a sphere of radius R that is visible from at least one
 * of two scanners (points outside the sphere).
 * 
 * Solution approach:
 * 1. A scanner at distance D from the center of a sphere of radius R can see a spherical cap.
 *    The height h of this cap is h = R(1 - R/D).
 *    The angular radius alpha of this cap is alpha = arccos(R/D).
 *    The area of this cap is 2 * PI * R^2 * (1 - cos(alpha)).
 * 2. Let the two caps be C1 and C2 with angular radii alpha1, alpha2 and the angular
 *    distance between their centers be gamma.
 * 3. The area of the union is Area(C1) + Area(C2) - Area(C1 intersect C2).
 * 4. The area of the intersection of two spherical caps can be calculated using the 
 *    Gauss-Bonnet Theorem for a region bounded by two small circle arcs:
 *    Area_int = R^2 * (2*PI - 2*beta - 2*phi1*cos(alpha1) - 2*phi2*cos(alpha2))
 *    where beta is the angle between the radii O1X and O2X (the center distance 
 *    of the intersection points), and phi1, phi2 are half the angular arc lengths
 *    of the boundaries in each cap.
 *    Specifically:
 *    beta = arccos((cos(gamma) - cos(alpha1)*cos(alpha2)) / (sin(alpha1)*sin(alpha2)))
 *    phi1 = arccos((cos(alpha2) - cos(alpha1)*cos(gamma)) / (sin(alpha1)*sin(gamma)))
 *    phi2 = arccos((cos(alpha1) - cos(alpha2)*cos(gamma)) / (sin(alpha2)*sin(gamma)))
 *    This formula works when the intersection is a proper region with two vertices.
 *    Edge cases:
 *    - gamma >= alpha1 + alpha2: Intersection area = 0.
 *    - gamma <= |alpha1 - alpha2|: Intersection area = min(Area(C1), Area(C2)).
 */

using namespace std;

typedef long double ld;

const ld PI = acosl(-1.0L);

// Utility to clamp a value into the range [-1.0, 1.0] for safe acos usage.
ld clamp_val(ld x) {
    if (x < -1.0L) return -1.0L;
    if (x > 1.0L) return 1.0L;
    return x;
}

int main() {
    ld R;
    if (!(cin >> R)) return 0;

    ld x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

    ld d1 = sqrtl(x1 * x1 + y1 * y1 + z1 * z1);
    ld d2 = sqrtl(x2 * x2 + y2 * y2 + z2 * z2);

    ld alpha1 = acosl(clamp_val(R / d1));
    ld alpha2 = acosl(clamp_val(R / d2));

    ld dot_product = (x1 * x2 + y1 * y2 + z1 * z2);
    ld cos_gamma = clamp_val(dot_product / (d1 * d2));
    ld gamma = acosl(cos_gamma);

    ld area1 = 2.0L * PI * R * R * (1.0L - cosl(alpha1));
    ld area2 = 2.0L * PI * R * R * (1.0L - cosl(alpha2));

    ld area_int = 0;

    if (gamma >= alpha1 + alpha2 - 1e-15L) {
        // No overlap
        area_int = 0;
    } else if (gamma <= fabsl(alpha1 - alpha2) + 1e-15L) {
        // One cap is inside the other
        area_int = min(area1, area2);
    } else {
        // Standard intersection formula
        ld cos_beta = clamp_val((cosl(gamma) - cosl(alpha1) * cosl(alpha2)) / (sinl(alpha1) * sinl(alpha2)));
        ld beta = acosl(cos_beta);

        ld cos_phi1 = clamp_val((cosl(alpha2) - cosl(alpha1) * cosl(gamma)) / (sinl(alpha1) * sinl(gamma)));
        ld phi1 = acosl(cos_phi1);

        ld cos_phi2 = clamp_val((cosl(alpha1) - cosl(alpha2) * cosl(gamma)) / (sinl(alpha2) * sinl(gamma)));
        ld phi2 = acosl(cos_phi2);

        area_int = R * R * (2.0L * PI - 2.0L * beta - 2.0L * phi1 * cosl(alpha1) - 2.0L * phi2 * cosl(alpha2));
    }

    ld total_area = area1 + area2 - area_int;

    // To handle potential floating point errors where total_area might slightly exceed 4*PI*R^2
    ld max_area = 4.0L * PI * R * R;
    if (total_area > max_area) total_area = max_area;

    cout << fixed << setprecision(3) << (double)total_area << endl;

    return 0;
}

