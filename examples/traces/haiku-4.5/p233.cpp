#include<bits/stdc++.h>
using namespace std;

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

double angle(double xa, double ya, double xc, double yc, double xb, double yb) {
    double ca = dist(xc, yc, xa, ya);
    double cb = dist(xc, yc, xb, yb);
    double ab = dist(xa, ya, xb, yb);
    
    if (ca < 1e-9 || cb < 1e-9) return 0;
    
    double cosAngle = (ca*ca + cb*cb - ab*ab) / (2*ca*cb);
    cosAngle = max(-1.0, min(1.0, cosAngle));
    return acos(cosAngle);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    while(n--) {
        double x0, y0, r, xa, ya, xb, yb;
        cin >> x0 >> y0 >> r >> xa >> ya >> xb >> yb;
        
        // Midpoint of AB
        double mx = (xa + xb) / 2;
        double my = (ya + yb) / 2;
        
        // Direction perpendicular to AB
        double dx = xb - xa;
        double dy = yb - ya;
        double len = sqrt(dx*dx + dy*dy);
        
        // Perpendicular direction (rotated 90 degrees)
        double px = -dy / len;
        double py = dx / len;
        
        // Find intersection of perpendicular bisector with circle
        // Point on line: (mx + t*px, my + t*py)
        // Circle equation: (mx + t*px - x0)^2 + (my + t*py - y0)^2 = r^2
        
        double a = px*px + py*py; // = 1
        double b = 2*((mx-x0)*px + (my-y0)*py);
        double c = (mx-x0)*(mx-x0) + (my-y0)*(my-y0) - r*r;
        
        double disc = b*b - 4*a*c;
        
        double t1 = (-b + sqrt(disc)) / (2*a);
        double t2 = (-b - sqrt(disc)) / (2*a);
        
        double xc1 = mx + t1*px;
        double yc1 = my + t1*py;
        double xc2 = mx + t2*px;
        double yc2 = my + t2*py;
        
        double ang1 = angle(xa, ya, xc1, yc1, xb, yb);
        double ang2 = angle(xa, ya, xc2, yc2, xb, yb);
        
        printf("%.6f %.6f\n", ang1 > ang2 ? xc1 : xc2, ang1 > ang2 ? yc1 : yc2);
    }
    
    return 0;
}
