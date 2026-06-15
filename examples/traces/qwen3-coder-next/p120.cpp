#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const double PI = acos(-1.0);

int main() {
    int N, N1, N2;
    cin >> N >> N1 >> N2;
    
    double x1, y1, x2, y2;
    cin >> x1 >> y1;
    cin >> x2 >> y2;
    
    // Convert to 0-based index
    N1--; 
    N2--;
    
    // Calculate center of the regular polygon
    // In a regular polygon, the center is equidistant from all vertices
    // We can find the center as the intersection of perpendicular bisectors
    
    // For a regular N-gon, the angle between consecutive vertices from the center is 2*PI/N
    // The angle between vertices N1 and N2 is k * 2*PI/N, where k = (N2 - N1 + N) % N
    
    int k = (N2 - N1 + N) % N;
    if (k > N / 2) k = N - k; // take the smaller arc
    
    // The central angle between N1 and N2
    double angle_between = 2.0 * PI * k / N;
    
    // Distance between islands N1 and N2
    double d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    
    // Using law of cosines: d^2 = R^2 + R^2 - 2*R*R*cos(angle_between)
    // d^2 = 2*R^2*(1 - cos(angle_between))
    // R = d / sqrt(2*(1 - cos(angle_between)))
    // But better to use: R = d / (2 * sin(angle_between/2))
    double R = d / (2 * sin(angle_between / 2));
    
    // Calculate the center (cx, cy)
    // The center lies on the perpendicular bisector of the segment between (x1,y1) and (x2,y2)
    // Midpoint: mx = (x1+x2)/2, my = (y1+y2)/2
    // Direction from (x1,y1) to (x2,y2): (dx, dy) = (x2-x1, y2-y1)
    // Perpendicular direction: (-dy, dx) or (dy, -dx)
    // Distance from midpoint to center: h = R * cos(angle_between/2)
    
    double mx = (x1 + x2) / 2.0;
    double my = (y1 + y2) / 2.0;
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    double dist_mid_to_center = R * cos(angle_between / 2.0);
    
    // Unit perpendicular vector
    double perp_len = sqrt(dx*dx + dy*dy);
    if (perp_len == 0) {
        // This shouldn't happen as N1 != N2
        perp_len = 1;
    }
    
    // Two possible centers: one on each side of the segment
    // We need to determine which one is correct
    // The polygon is oriented clockwise, so we need to check the orientation
    
    // Calculate both possible centers
    double cx1 = mx + dist_mid_to_center * (-dy) / perp_len;
    double cy1 = my + dist_mid_to_center * (dx) / perp_len;
    
    double cx2 = mx + dist_mid_to_center * (dy) / perp_len;
    double cy2 = my + dist_mid_to_center * (-dx) / perp_len;
    
    // Determine correct center by checking the orientation
    // For a clockwise polygon, the center should be to the right of the vector from N1 to N2
    // Vector from N1 to N2: (dx, dy)
    // Vector from N1 to center1: (cx1-x1, cy1-y1)
    // Cross product: dx*(cy1-y1) - dy*(cx1-x1)
    // If positive, center1 is to the left; if negative, to the right
    
    double cross1 = dx * (cy1 - y1) - dy * (cx1 - x1);
    double cross2 = dx * (cy2 - y1) - dy * (cx2 - x1);
    
    // For clockwise order, the center should be to the right of the directed edge
    // So we want the center where the cross product is negative (for standard coordinate system)
    // But let's verify with the sample: N=4, N1=1, N2=3, points (1,0) and (1,2)
    // Vector from N1 to N2: (0, 2)
    // Midpoint: (1, 1)
    // k = (3-1) % 4 = 2, angle_between = 2*PI*2/4 = PI, so R = d/2 = 1
    // dist_mid_to_center = 1 * cos(PI/2) = 0, so center should be at midpoint (1,1)
    // But wait, for a square, the center is at (1,1) if points are (1,0) and (1,2)
    // Actually, in the sample, the islands are at (1,0), (0,1), (1,2), (2,1)
    // So the center is at (1,1)
    
    // Let's compute both centers for the sample:
    // dx=0, dy=2, perp_len=2
    // center1: mx + 0*(-2)/2 = 1, my + 0*(0)/2 = 1 -> (1,1)
    // center2: mx + 0*(2)/2 = 1, my + 0*(0)/2 = 1 -> (1,1)
    // Hmm, when angle_between = PI, dist_mid_to_center = 0, so both centers coincide.
    
    // For a square (N=4), the central angle between opposite vertices is PI, so they're diametrically opposite.
    // In this case, the center is exactly the midpoint.
    
    // For other cases, we need to figure out which side is correct.
    // Since the polygon is clockwise, the vertices go in clockwise order.
    // The rotation from the vector (center -> N1) to (center -> N2) should be clockwise, i.e., negative angle.
    
    // Let's try center1 and center2 and see which one gives the correct ordering.
    // We can compute the angle of each point relative to the center and check if increasing index corresponds to clockwise rotation.
    
    double cx, cy;
    
    // For regular polygons with even number of sides and N1, N2 opposite, center is midpoint.
    // For odd or non-opposite, we need to choose correctly.
    // Let's use the fact that the polygon is clockwise.
    // The angle from center to N1 should be greater than angle from center to N2 by angle_between (mod 2PI) for clockwise.
    
    // Try center1
    auto get_angle = [](double cx, double cy, double x, double y) {
        return atan2(y - cy, x - cx);
    };
    
    double angle1_1 = get_angle(cx1, cy1, x1, y1);
    double angle1_2 = get_angle(cx1, cy1, x2, y2);
    double diff1 = angle1_1 - angle1_2;
    while (diff1 <= 0) diff1 += 2 * PI;
    diff1 = fmod(diff1, 2 * PI);
    
    double angle2_1 = get_angle(cx2, cy2, x1, y1);
    double angle2_2 = get_angle(cx2, cy2, x2, y2);
    double diff2 = angle2_1 - angle2_2;
    while (diff2 <= 0) diff2 += 2 * PI;
    diff2 = fmod(diff2, 2 * PI);
    
    // For clockwise order, as we go from N1 to N2, we move clockwise, so the angle should decrease by angle_between.
    // So diff should be approximately angle_between (mod 2PI)
    double target_diff = fmod(angle_between, 2 * PI);
    if (target_diff < 0) target_diff += 2 * PI;
    
    if (abs(diff1 - target_diff) < 1e-5 || abs(diff1 - (2*PI - target_diff)) < 1e-5) {
        // But for clockwise, diff should be positive and equal to angle_between (if we consider the smaller rotation)
        // Actually, in clockwise, the angle difference (N1 angle - N2 angle) should be angle_between (mod 2PI)
        // Let's check the sample: N=4, angle_between = PI
        // For center (1,1): 
        // N1 (1,0): angle = atan2(-1, 0) = -PI/2 or 3PI/2
        // N2 (1,2): angle = atan2(1, 0) = PI/2
        // diff = 3PI/2 - PI/2 = PI, which matches angle_between.
        // So we want diff = angle_between.
        
        if (abs(diff1 - target_diff) < 1e-5) {
            cx = cx1;
            cy = cy1;
        } else {
            cx = cx2;
            cy = cy2;
        }
    } else {
        cx = cx2;
        cy = cy2;
    }
    
    // Now we have the center (cx, cy)
    // Calculate the starting angle for island 0 (index 0, which is island 1 in 1-based)
    // Island 0 corresponds to island N1 (0-based index N1)
    double angle_N1 = atan2(y1 - cy, x1 - cx);
    
    // The angle for island i (0-based) should be angle_N1 - i * (2*PI/N) for clockwise
    // But wait, island N1 is at index N1 (0-based), so for island i (0-based), 
    // the angle should be angle_N1 - (i - N1) * (2*PI/N)
    // Normalize to [0, 2*PI) if needed, but atan2 can handle negative angles.
    
    vector<pair<double, double>> islands(N);
    
    for (int i = 0; i < N; i++) {
        // Angle for island i (0-based)
        double angle_i = angle_N1 - (i - N1) * (2.0 * PI / N);
        
        // Compute coordinates
        double x = cx + R * cos(angle_i);
        double y = cy + R * sin(angle_i);
        
        islands[i] = {x, y};
    }
    
    // Output with 6 decimal places
    cout << fixed << setprecision(6);
    for (int i = 0; i < N; i++) {
        cout << islands[i].first << " " << islands[i].second << endl;
    }
    
    return 0;
}
