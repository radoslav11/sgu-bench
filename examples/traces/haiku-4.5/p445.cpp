#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    double vw, vc;
    cin >> n >> vw >> vc;
    
    vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }
    
    double total_time = 0.0;
    
    for (int i = 0; i < n - 1; i++) {
        double dx = x[i + 1] - x[i];
        double dy = y[i + 1] - y[i];
        
        // Distance along the surface (straight line)
        double line_distance = sqrt(dx * dx + dy * dy);
        double walk_time = line_distance / vw;
        
        // Horizontal tunneling
        double tunnel_time = fabs(dx) / vc;
        
        // Choose the minimum time for this segment
        total_time += min(walk_time, tunnel_time);
    }
    
    cout << fixed << setprecision(6) << total_time << endl;
    
    return 0;
}
