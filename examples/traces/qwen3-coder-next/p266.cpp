#include <iostream>
#include <cmath>
#include <iomanip>

const double PI = acos(-1.0);

struct Vector3D {
    double x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }
    
    Vector3D normalize() const {
        double n = norm();
        return Vector3D(x / n, y / n, z / n);
    }
};

double distance(const Vector3D& a, const Vector3D& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + 
                (a.y - b.y)*(a.y - b.y) + 
                (a.z - b.z)*(a.z - b.z));
}

int main() {
    double R;
    std::cin >> R;
    
    Vector3D scanner1, scanner2;
    std::cin >> scanner1.x >> scanner1.y >> scanner1.z;
    std::cin >> scanner2.x >> scanner2.y >> scanner2.z;
    
    // Distance from center to each scanner
    double d1 = scanner1.norm();
    double d2 = scanner2.norm();
    
    // Check visibility: a point on sphere is visible from scanner if angle between 
    // vector from scanner to point and vector from scanner to center is < 90 degrees
    
    // For a sphere of radius R and scanner at distance d from center, the visible region 
    // is a spherical cap with angular radius alpha where sin(alpha) = R / d
    // The area of a spherical cap is 2 * PI * R^2 * (1 - cos(alpha))
    // Since sin(alpha) = R/d, we have cos(alpha) = sqrt(1 - (R/d)^2)
    
    // Calculate angular radius for each scanner's visibility cap
    double alpha1 = asin(R / d1);
    double alpha2 = asin(R / d2);
    
    // Areas of individual caps
    double cap1_area = 2.0 * PI * R * R * (1.0 - cos(alpha1));
    double cap2_area = 2.0 * PI * R * R * (1.0 - cos(alpha2));
    
    // Distance between scanners
    double d = distance(scanner1, scanner2);
    
    // Now compute intersection area of the two caps
    // For two spherical caps, the intersection is non-empty only if the angular distance
    // between the cap centers is less than alpha1 + alpha2
    
    // The centers of the caps are the points on the sphere closest to each scanner,
    // which are in the same direction as the scanners from the center.
    // So the angular distance between cap centers is the angle between scanner1 and scanner2 vectors.
    double angle_between = acos(scanner1.dot(scanner2) / (d1 * d2));
    
    double intersection_area = 0.0;
    
    // If caps don't overlap
    if (angle_between >= alpha1 + alpha2) {
        intersection_area = 0.0;
    }
    // If one cap is completely inside the other
    else if (angle_between <= fabs(alpha1 - alpha2)) {
        intersection_area = 2.0 * PI * R * R * (1.0 - cos(min(alpha1, alpha2)));
    }
    // Partial overlap
    else {
        // Formula for intersection area of two spherical caps
        // Based on spherical geometry
        double cos_alpha1 = cos(alpha1);
        double cos_alpha2 = cos(alpha2);
        double cos_angle = cos(angle_between);
        double sin_alpha1 = sin(alpha1);
        double sin_alpha2 = sin(alpha2);
        double sin_angle = sin(angle_between);
        
        // Using formula from spherical geometry for lens-shaped intersection
        double term1 = (1.0 - cos_alpha1) + (1.0 - cos_alpha2);
        // More robust approach using spherical triangle formulas
        
        // Alternative formula: 
        // h1 = R * (1 - cos(alpha1)), h2 = R * (1 - cos(alpha2))
        // But for spheres, the formula is:
        
        // From spherical cap intersection area formula:
        double phi1 = acos((cos_alpha1 - cos_alpha2 * cos_angle) / (sin_alpha2 * sin_angle));
        double phi2 = acos((cos_alpha2 - cos_alpha1 * cos_angle) / (sin_alpha1 * sin_angle));
        
        intersection_area = R * R * (phi1 * (1 - cos_alpha1) + phi2 * (1 - cos_alpha2) - 
                                    (phi1 * sin_alpha1 * sin_alpha1 + phi2 * sin_alpha2 * sin_alpha2) / 2.0 +
                                    (sin(2*alpha1) * phi1 + sin(2*alpha2) * phi2) / 4.0);
        
        // Actually, let's use a more standard formula:
        // area = R^2 * (theta1 + theta2 - sin(theta1 + theta2))
        // where theta1 and theta2 are related to the cap angles and separation
        
        // Better approach: use formula from "Area of intersection of two spherical caps"
        // Let d = angular distance between centers, r1 = alpha1, r2 = alpha2
        // If |r1 - r2| < d < r1 + r2, then:
        double h1 = R * (1 - cos_alpha1);
        double h2 = R * (1 - cos_alpha2);
        
        // The intersection area can be computed as sum of two circular segments
        // For each cap, we need the height of the segment forming the intersection
        
        // Distance from center of cap1 to the chord (intersection circle plane)
        // Using spherical law of cosines
        double cos_delta1 = (cos_alpha2 - cos_alpha1 * cos_angle) / (sin_alpha1 * sin_angle);
        double delta1 = acos(cos_delta1);
        double cos_delta2 = (cos_alpha1 - cos_alpha2 * cos_angle) / (sin_alpha2 * sin_angle);
        double delta2 = acos(cos_delta2);
        
        // Spherical segment areas
        intersection_area = R * R * ((delta1 * (1 - cos_alpha1)) + (delta2 * (1 - cos_alpha2)));
    }
    
    // Final answer: union area = cap1 + cap2 - intersection
    double total_area = cap1_area + cap2_area - intersection_area;
    
    std::cout << std::fixed << std::setprecision(3) << total_area << std::endl;
    
    return 0;
}
