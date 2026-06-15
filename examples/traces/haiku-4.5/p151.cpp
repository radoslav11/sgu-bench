#include <bits/stdc++.h>
using namespace std;

int main(){
    double c, b, m;
    cin >> c >> b >> m;
    
    // Check triangle inequality for median
    // Using formula: 4m² = 2b² + 2c² - a² where a = |BC|
    // So a² = 2b² + 2c² - 4m²
    
    double a_squared = 2*b*b + 2*c*c - 4*m*m;
    
    if(a_squared < 0){
        cout << "Mission impossible" << endl;
        return 0;
    }
    
    double a = sqrt(a_squared);
    
    // Place A at origin, B on positive x-axis
    double Ax = 0, Ay = 0;
    double Bx = c, By = 0;
    
    // Find C such that |AC| = b and |BC| = a
    // x² + y² = b²
    // (x-c)² + y² = a²
    // x² - 2cx + c² + y² = a²
    // b² - 2cx + c² = a²
    // x = (b² + c² - a²)/(2c)
    
    double Cx = (b*b + c*c - a*a)/(2*c);
    double Cy_squared = b*b - Cx*Cx;
    
    if(Cy_squared < -1e-9){
        cout << "Mission impossible" << endl;
        return 0;
    }
    
    if(Cy_squared < 0) Cy_squared = 0;
    double Cy = sqrt(Cy_squared);
    
    cout << fixed << setprecision(5);
    cout << Ax << " " << Ay << endl;
    cout << Bx << " " << By << endl;
    cout << Cx << " " << Cy << endl;
    
    return 0;
}
