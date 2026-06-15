#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;

struct Circle {
    double x, y;
    double vx, vy;
    int r, m;
};

int main() {
    Circle A, B;
    cin >> A.x >> A.y >> A.vx >> A.vy >> A.r >> A.m;
    cin >> B.x >> B.y >> B.vx >> B.vy >> B.r >> B.m;
    int t;
    cin >> t;

    // Compute initial distance between centers
    double dx = B.x - A.x;
    double dy = B.y - A.y;
    double dist0 = sqrt(dx*dx + dy*dy);
    double min_dist = A.r + B.r;

    // Check if circles will collide during time [0, t]
    // Relative velocity
    double vdx = B.vx - A.vx;
    double vdy = B.vy - A.vy;
    
    // Distance squared as function of time: |(B0-A0) + (vB-vA)*t|^2
    // We want to know if there's a time s in [0, t] where distance = min_dist
    // Solve: |dx + vdx*s|^2 + |dy + vdy*s|^2 = min_dist^2
    // => (vdx^2 + vdy^2)*s^2 + 2*(dx*vdx + dy*vdy)*s + (dx^2 + dy^2 - min_dist^2) = 0
    
    double a = vdx*vdx + vdy*vdy;
    double b = 2*(dx*vdx + dy*vdy);
    double c = dx*dx + dy*dy - min_dist*min_dist;
    
    double collision_time = -1.0;
    
    // If a == 0, relative velocity is zero, no collision possible (guaranteed no initial collision)
    if (a == 0) {
        // No relative movement, so no collision
        collision_time = -1;
    } else {
        double discriminant = b*b - 4*a*c;
        if (discriminant >= 0) {
            double sqrt_disc = sqrt(discriminant);
            double s1 = (-b - sqrt_disc) / (2*a);
            double s2 = (-b + sqrt_disc) / (2*a);
            
            // Find smallest non-negative time within [0, t]
            if (s1 >= 0 && s1 <= t) {
                collision_time = s1;
            } else if (s2 >= 0 && s2 <= t) {
                collision_time = s2;
            }
        }
    }
    
    // If collision occurs, simulate before and after collision
    if (collision_time >= 0 && collision_time <= t) {
        // Step 1: Move both circles for 'collision_time' time without collision
        A.x += A.vx * collision_time;
        A.y += A.vy * collision_time;
        B.x += B.vx * collision_time;
        B.y += B.vy * collision_time;
        
        // Step 2: Calculate collision
        // Vector from A to B
        dx = B.x - A.x;
        dy = B.y - A.y;
        double dist = sqrt(dx*dx + dy*dy);
        
        // Normalize collision normal vector (from A to B)
        double nx = dx / dist;
        double ny = dy / dist;
        
        // Project velocities onto the normal direction
        double vAn = A.vx * nx + A.vy * ny;
        double vBn = B.vx * nx + B.vy * ny;
        
        // Tangential components remain unchanged
        // For elastic collision: use 1D elastic collision formulas along the normal
        double total_m = A.m + B.m;
        double new_vAn = ( (A.m - B.m) * vAn + 2 * B.m * vBn ) / total_m;
        double new_vBn = ( (B.m - A.m) * vBn + 2 * A.m * vAn ) / total_m;
        
        // Update normal components of velocities
        double new_vAx = A.vx + (new_vAn - vAn) * nx;
        double new_vAy = A.vy + (new_vAn - vAn) * ny;
        double new_vBx = B.vx + (new_vBn - vBn) * nx;
        double new_vBy = B.vy + (new_vBn - vBn) * ny;
        
        A.vx = new_vAx;
        A.vy = new_vAy;
        B.vx = new_vBx;
        B.vy = new_vBy;
        
        // Step 3: Move remaining time (t - collision_time) without further collision
        double remaining_time = t - collision_time;
        A.x += A.vx * remaining_time;
        A.y += A.vy * remaining_time;
        B.x += B.vx * remaining_time;
        B.y += B.vy * remaining_time;
    } else {
        // No collision, just move for time t
        A.x += A.vx * t;
        A.y += A.vy * t;
        B.x += B.vx * t;
        B.y += B.vy * t;
    }
    
    cout << fixed << setprecision(3);
    cout << A.x << " " << A.y << " " << A.vx << " " << A.vy << "\n";
    cout << B.x << " " << B.y << " " << B.vx << " " << B.vy << "\n";
    
    return 0;
}
