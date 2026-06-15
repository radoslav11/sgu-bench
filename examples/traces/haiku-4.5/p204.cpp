#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e9;

double b1, t1, b2, t2, l, ds, df, g;

// Check if we can jump from (0, y0) to (dist, y_final) passing through hole at (hole_x, [hole_b, hole_t])
// with max speed v_max
bool canJump(double y0, double dist, double hole_x, double hole_b, double hole_t, 
             double y_final_min, double y_final_max, double v_max) {
    // Try to find a valid trajectory
    // Parametrize by angle theta and find required speed
    
    for(double theta = -M_PI/2; theta <= M_PI/2; theta += 0.01) {
        double cos_t = cos(theta);
        double sin_t = sin(theta);
        
        if(cos_t < EPS) continue; // Not moving forward enough
        
        // Time to reach hole_x
        // hole_x = v * cos_t * t1
        // We need to find v such that trajectory passes through hole
        
        double t_hole = hole_x / (v_max * cos_t);
        double y_at_hole = y0 + v_max * sin_t * t_hole - 0.5 * g * t_hole * t_hole;
        
        if(y_at_hole < hole_b - EPS || y_at_hole > hole_t + EPS) continue;
        
        // Now check if we can land at dist
        // dist = v * cos_t * t_final
        double t_final = dist / (v_max * cos_t);
        double y_final = y0 + v_max * sin_t * t_final - 0.5 * g * t_final * t_final;
        
        if(y_final >= y_final_min - EPS && y_final <= y_final_max + EPS) {
            return true;
        }
    }
    
    return false;
}

bool solve(double v_max) {
    // First jump: from (0, 0) through hole at (ds, [b1, t1]) to (ds+l, y1)
    // Second jump: from (ds+l, y1) through hole at (ds+2l, [b2, t2]) to (ds+2l+df, y_final)
    
    // Try many landing heights for first jump
    for(double y1 = b2 - 100; y1 <= t2 + 100; y1 += 0.1) {
        if(canJump(0, ds, ds, b1, t1, y1 - 0.01, y1 + 0.01, v_max)) {
            // Can we complete second jump?
            if(canJump(y1, ds + l, ds + l, b2, t2, -INF, INF, v_max)) {
                return true;
            }
        }
    }
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    while(cin >> b1 >> t1 >> b2 >> t2 >> l >> ds >> df >> g) {
        double left = 0, right = 1000;
        double ans = -1;
        
        for(int iter = 0; iter < 100; iter++) {
            double mid = (left + right) / 2;
            if(solve(mid)) {
                ans = mid;
                right = mid;
            } else {
                left = mid;
            }
        }
        
        if(ans < 0 || ans > 999) {
            cout << -1 << "\n";
        } else {
            cout << fixed << setprecision(4) << ans << "\n";
        }
    }
    
    return 0;
}
