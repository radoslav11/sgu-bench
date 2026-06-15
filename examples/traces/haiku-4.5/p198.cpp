#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e9;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<double> x(n), y(n), r(n);
    for(int i = 0; i < n; i++){
        cin >> x[i] >> y[i] >> r[i];
    }
    
    double kx, ky, kr;
    cin >> kx >> ky >> kr;
    
    // Check many directions
    int samples = 3600;
    bool canEscape = false;
    
    for(int dir = 0; dir < samples; dir++){
        double angle = 2.0 * M_PI * dir / samples;
        double dx = cos(angle);
        double dy = sin(angle);
        
        // Try to move in direction (dx, dy) as far as possible
        double maxDist = INF;
        
        for(int i = 0; i < n; i++){
            // Vector from Kolobok center to island center
            double ox = x[i] - kx;
            double oy = y[i] - ky;
            
            // Distance along direction (dx, dy) where Kolobok would touch island i
            // We're moving center along (dx, dy) starting from (0, 0)
            // Island at (ox, oy) with radius r[i]
            // Kolobok has radius kr
            // Collision when: distance from (t*dx, t*dy) to (ox, oy) < r[i] + kr
            
            double a = dx*dx + dy*dy; // = 1
            double b = 2*(dx*ox + dy*oy);
            double c = ox*ox + oy*oy - (r[i] + kr)*(r[i] + kr);
            
            double disc = b*b - 4*a*c;
            if(disc >= 0){
                double t1 = (-b - sqrt(disc)) / (2*a);
                double t2 = (-b + sqrt(disc)) / (2*a);
                
                // We want the first positive t where we hit
                if(t2 > EPS){
                    maxDist = min(maxDist, t2);
                }
            }
        }
        
        // If we can move very far in this direction, we can escape
        if(maxDist > 1e6){
            canEscape = true;
            break;
        }
    }
    
    if(canEscape){
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
