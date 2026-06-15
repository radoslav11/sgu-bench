#include<bits/stdc++.h>
using namespace std;

int main(){
    double R;
    cin >> R;
    
    double x1, y1, z1, x2, y2, z2;
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    
    double d1 = sqrt(x1*x1 + y1*y1 + z1*z1);
    double d2 = sqrt(x2*x2 + y2*y2 + z2*z2);
    
    // For each scanner, calculate the visible spherical cap
    // Height of cap: h = R - sqrt(d^2 - R^2) is wrong
    // Correct: if scanner at distance d, angle to horizon is arcsin(R/d)
    // Height of spherical cap: h = R(1 - cos(arcsin(R/d))) = R(1 - sqrt(1 - R^2/d^2))
    
    double h1 = R * (1.0 - sqrt(1.0 - R*R/(d1*d1)));
    double h2 = R * (1.0 - sqrt(1.0 - R*R/(d2*d2)));
    
    double area1 = 2.0 * M_PI * R * h1;
    double area2 = 2.0 * M_PI * R * h2;
    
    // Calculate the angle between the two scanners as seen from planet center
    double dot = x1*x2 + y1*y2 + z1*z2;
    double cos_angle = dot / (d1 * d2);
    double angle = acos(max(-1.0, min(1.0, cos_angle)));
    
    // Calculate intersection area using formula for two spherical caps
    double alpha1 = asin(R / d1);
    double alpha2 = asin(R / d2);
    
    double intersection = 0.0;
    if (angle <= alpha1 + alpha2) {
        // Caps overlap
        double h_intersect1 = R * (1.0 - cos(alpha1 - angle));
        double h_intersect2 = R * (1.0 - cos(alpha2 - angle));
        if (angle >= fabs(alpha1 - alpha2)) {
            double h = R * (1.0 - (cos(alpha1)*cos(alpha2) - sin(alpha1)*sin(alpha2)*cos(angle))/(sin(alpha1)*sin(alpha2)));
            intersection = 2.0 * M_PI * R * h;
        } else {
            intersection = min(area1, area2);
        }
    }
    
    double total = area1 + area2 - intersection;
    
    cout << fixed << setprecision(3) << total << endl;
    
    return 0;
}
