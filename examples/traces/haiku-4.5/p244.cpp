#include<bits/stdc++.h>
using namespace std;

int main(){
    double h, d, m;
    cin >> h >> d >> m;
    
    // A is at (0, h), BC is on y=0
    // B = (-b, 0), C = (c, 0)
    // M = ((c-b)/2, 0): |AM|^2 = ((c-b)/2)^2 + h^2 = m^2
    // So (c-b)^2 = 4(m^2 - h^2)
    
    // Try to find b, c by search
    if(m < h - 1e-9) {
        cout << "NO\n";
        return 0;
    }
    
    double half_sum = sqrt(max(0.0, m*m - h*h));
    
    // (c-b) = 2*half_sum
    // Try different values of b and c
    
    for(double b = 0.01; b <= 100; b += 0.01) {
        double c = b + 2*half_sum;
        if(c < 0.01) continue;
        
        double pab = sqrt(b*b + h*h);
        double pac = sqrt(c*c + h*h);
        
        // D position on BC
        double d_pos = (-b*pac + c*pab)/(pab + pac);
        double ad_len = sqrt(d_pos*d_pos + h*h);
        
        if(fabs(ad_len - d) < 1e-6) {
            cout << fixed << setprecision(8);
            cout << "YES\n";
            cout << "0 " << h << "\n";
            cout << -b << " 0\n";
            cout << c << " 0\n";
            return 0;
        }
    }
    
    cout << "NO\n";
    return 0;
}
