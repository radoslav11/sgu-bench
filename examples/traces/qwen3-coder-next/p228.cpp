#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-9;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

int main() {
    int N, N1, N2;
    cin >> N >> N1 >> N2;
    
    double x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    
    Point p1(x1, y1), p2(x2, y2);
    
    // Convert to 0-indexed
    N1--; N2--;
    
    // The islands form a regular N-gon, so the central angle between consecutive islands is 2*PI/N
    // The angular distance between island N1 and N2 is |N2 - N1| steps, but we need the minimal angle
    int diff = abs(N2 - N1);
    int steps = min(diff, N - diff);
    
    // The central angle between p1 and p2 is theta = steps * (2*PI / N)
    double theta = steps * 2 * PI / N;
    
    // Distance between p1 and p2
    double d = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    
    // In a regular polygon, chord length = 2 * R * sin(theta/2)
    // So R = d / (2 * sin(theta/2))
    double R = d / (2 * sin(theta / 2));
    
    // Calculate the center of the polygon
    // The center lies on the perpendicular bisector of segment p1p2
    // Midpoint of p1p2
    double mx = (p1.x + p2.x) / 2.0;
    double my = (p1.y + p2.y) / 2.0;
    
    // Distance from midpoint to center: h = sqrt(R^2 - (d/2)^2)
    double h = sqrt(max(0.0, R * R - (d/2) * (d/2)));
    
    // Direction vector from p1 to p2
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    
    // Perpendicular direction (rotated 90 degrees)
    // Two possible centers: one on each side of the segment
    // We need to determine which one gives the correct polygon orientation (clockwise numbering)
    
    // Normalize perpendicular vector
    double perp_x = -dy / d;
    double perp_y = dx / d;
    
    // Two candidate centers
    Point center1(mx + perp_x * h, my + perp_y * h);
    Point center2(mx - perp_x * h, my - perp_y * h);
    
    // To determine which center is correct, we can check the angular order
    // For clockwise numbering, the angle from center to islands should decrease (or increase, depending on coordinate system)
    // In standard Cartesian coordinates, counterclockwise is positive angle direction
    // Since islands are numbered clockwise, we expect the angle to decrease as index increases
    
    // Let's define a function to compute angle from center to point (in radians, from -PI to PI)
    auto angle_from_center = [](const Point& center, const Point& p) {
        return atan2(p.y - center.y, p.x - center.x);
    };
    
    double angle_p1_1 = angle_from_center(center1, p1);
    double angle_p2_1 = angle_from_center(center1, p2);
    
    double angle_p1_2 = angle_from_center(center2, p1);
    double angle_p2_2 = angle_from_center(center2, p2);
    
    // For clockwise numbering, if N2 > N1 (in the circular sense), then the angle at N2 should be 
    // less than angle at N1 by the angular step (modulo 2*PI)
    // The angular difference when going from N1 to N2 clockwise should be -steps * (2*PI/N)
    // Or equivalently, angle(N2) = angle(N1) - steps * (2*PI/N) (mod 2*PI)
    
    // Compute the expected angular difference (clockwise means negative change)
    double expected_diff = -steps * 2 * PI / N;
    
    // Normalize function to [-PI, PI]
    auto normalize_angle = [](double a) {
        while (a <= -PI) a += 2*PI;
        while (a > PI) a -= 2*PI;
        return a;
    };
    
    // For center1: compute angle difference
    double diff1 = normalize_angle(angle_p2_1 - angle_p1_1);
    // For center2: compute angle difference
    double diff2 = normalize_angle(angle_p2_2 - angle_p1_2);
    
    Point center;
    if (abs(diff1 - expected_diff) < abs(diff2 - expected_diff)) {
        center = center1;
    } else {
        center = center2;
    }
    
    // Now we have the center. The angular step between consecutive islands is 2*PI/N (but clockwise, so negative)
    // Calculate angle of p1 relative to center
    double base_angle = atan2(p1.y - center.y, p1.x - center.x);
    
    // The angle for island i (0-indexed) should be: base_angle - (i - N1) * (2*PI/N)
    // We need to handle circular indexing
    vector<Point> islands(N);
    
    for (int i = 0; i < N; i++) {
        // Calculate angle for island i
        // The angular difference between island N1 and island i is (N1 - i) steps clockwise
        double angle = base_angle - (N1 - i) * 2 * PI / N;
        
        // Normalize angle to avoid precision issues
        while (angle <= -PI) angle += 2*PI;
        while (angle > PI) angle -= 2*PI;
        
        islands[i].x = center.x + R * cos(angle);
        islands[i].y = center.y + R * sin(angle);
    }
    
    // Output in order (island 1 is at index 0 in our array)
    cout << fixed << setprecision(6);
    for (int i = 0; i < N; i++) {
        // Add EPS to handle very small negative numbers that should be zero
        double x = islands[i].x;
        double y = islands[i].y;
        if (abs(x) < EPS) x = 0.0;
        if (abs(y) < EPS) y = 0.0;
        cout << x << " " << y << endl;
    }
    
    return 0;
}
