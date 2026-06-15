#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

const double PI = acos(-1.0);

struct Circle {
    double x, y, r;
};

double intersectionArea(const Circle& c1, const Circle& c2) {
    double dx = c1.x - c2.x;
    double dy = c1.y - c2.y;
    double d_squared = dx * dx + dy * dy;
    double d = sqrt(d_squared);
    
    // If circles are separate or touch externally
    if (d >= c1.r + c2.r) {
        return 0.0;
    }
    
    // If one circle is completely inside the other
    if (d <= abs(c1.r - c2.r)) {
        double r_min = min(c1.r, c2.r);
        return PI * r_min * r_min;
    }
    
    // General case: two intersection points
    double r1 = c1.r;
    double r2 = c2.r;
    
    // Using the formula with acos and sqrt
    double part1 = r1 * r1 * acos((d * d + r1 * r1 - r2 * r2) / (2 * d * r1));
    double part2 = r2 * r2 * acos((d * d + r2 * r2 - r1 * r1) / (2 * d * r2));
    double part3 = 0.5 * sqrt((-d + r1 + r2) * (d + r1 - r2) * (d - r1 + r2) * (d + r1 + r2));
    
    return part1 + part2 - part3;
}

int main() {
    int n;
    cin >> n;
    vector<Circle> circles(n);
    for (int i = 0; i < n; i++) {
        cin >> circles[i].x >> circles[i].y >> circles[i].r;
    }

    double S1 = 0.0;
    for (int i = 0; i < n; i++) {
        S1 += PI * circles[i].r * circles[i].r;
    }

    double S2 = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            S2 += intersectionArea(circles[i], circles[j]);
        }
    }

    double burned = S1 - 2 * S2;
    double inverse = S2;

    cout << fixed << setprecision(5) << burned << " " << inverse << endl;
    return 0;
}
