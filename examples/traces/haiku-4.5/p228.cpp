#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, N1, N2;
    double x1, y1, x2, y2;
    
    cin >> N >> N1 >> N2 >> x1 >> y1 >> x2 >> y2;
    
    // Convert to 0-indexed
    N1--; N2--;
    
    // Calculate distance between the two known points
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dist = sqrt(dx*dx + dy*dy);
    
    // Angle between N1 and N2 in the polygon
    int angleDiff = abs(N2 - N1);
    // But we need the shorter arc
    if(angleDiff > N/2) angleDiff = N - angleDiff;
    
    double angle = angleDiff * 2 * M_PI / N;
    
    // Calculate radius using law of cosines
    double R = dist / sqrt(2 * (1 - cos(angle)));
    
    // Find center - it's on perpendicular bisector of the two points
    // Perpendicular bisector passes through midpoint
    double mx = (x1 + x2) / 2;
    double my = (y1 + y2) / 2;
    
    // Direction perpendicular to segment P1-P2
    double px = -dy / dist;
    double py = dx / dist;
    
    // Distance from midpoint to center
    double h = sqrt(R*R - (dist/2)*(dist/2));
    
    // Two possible centers
    double cx1 = mx + h * px;
    double cy1 = my + h * py;
    double cx2 = mx - h * px;
    double cy2 = my - h * py;
    
    // Choose the correct center (try both and see which gives correct angle orientation)
    double cx = cx1, cy = cy1;
    
    // Calculate angle of first island from center
    double angle1 = atan2(y1 - cy, x1 - cx);
    
    // Check if we need the other center
    double angle2 = atan2(y2 - cy, x2 - cx);
    double angleDiff_calc = angle2 - angle1;
    
    // Normalize angle difference
    while(angleDiff_calc < 0) angleDiff_calc += 2*M_PI;
    while(angleDiff_calc >= 2*M_PI) angleDiff_calc -= 2*M_PI;
    
    int expectedDiff = (N2 - N1 + N) % N;
    if(expectedDiff > N/2) expectedDiff = N - expectedDiff;
    
    if(abs(angleDiff_calc - expectedDiff * 2*M_PI/N) > abs(angleDiff_calc - (N - expectedDiff) * 2*M_PI/N)){
        cx = cx2; cy = cy2;
        angle1 = atan2(y1 - cy, x1 - cx);
    }
    
    cout << fixed << setprecision(6);
    for(int i = 0; i < N; i++){
        double angle_i = angle1 + i * 2 * M_PI / N;
        double xi = cx + R * cos(angle_i);
        double yi = cy + R * sin(angle_i);
        cout << xi << " " << yi << "\n";
    }
    
    return 0;
}
