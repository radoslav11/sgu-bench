#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long s_in, m_in, p_in;
    if (cin >> s_in >> m_in >> p_in) {
        double s = static_cast<double>(s_in);
        double m = static_cast<double>(m_in);
        double p = static_cast<double>(p_in);
        
        if (p == 0.0) {
            // If interest rate is 0, the monthly payment is simply the total divided by months
            cout << fixed << setprecision(6) << s / m << "\n";
        } else {
            // r is the monthly growth factor of the debt
            double r = 1.0 + p / 100.0;
            
            // rm is r raised to the power of m
            double rm = pow(r, m);
            
            // Calculate the exact monthly payment x using the annuity formula
            double x = s * rm * (r - 1.0) / (rm - 1.0);
            
            cout << fixed << setprecision(6) << x << "\n";
        }
    }
    
    return 0;
}
