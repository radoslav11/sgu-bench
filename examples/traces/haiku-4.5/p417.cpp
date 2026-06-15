#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double x0, y0, r;
    cin >> x0 >> y0 >> r;
    
    // Numerical integration using Gaussian quadrature
    // We'll use a simple but accurate approach with adaptive or fine grid
    
    double mass = 0.0;
    int n_theta = 1000;
    int n_s = 1000;
    
    double d_theta = 2.0 * M_PI / n_theta;
    double d_s = r / n_s;
    
    for(int i = 0; i < n_theta; i++){
        double theta = (i + 0.5) * d_theta;
        double cos_theta = cos(theta);
        double sin_theta = sin(theta);
        
        for(int j = 0; j < n_s; j++){
            double s = (j + 0.5) * d_s;
            
            // Calculate x^2 + y^2
            double x = x0 + s * cos_theta;
            double y = y0 + s * sin_theta;
            double dist_sq = x * x + y * y;
            
            // Density is ln(x^2 + y^2)
            double density = log(dist_sq);
            
            // Integrate: density * s * ds * dtheta
            mass += density * s * d_s * d_theta;
        }
    }
    
    cout << fixed << setprecision(12) << mass << endl;
    
    return 0;
}
