#include<bits/stdc++.h>
using namespace std;

int main(){
    double xA, yA, zA, xB, yB, zB;
    cin >> xA >> yA >> zA >> xB >> yB >> zB;
    
    // Compute the Lorentz product
    double lorentz_product = xA * xB + yA * yB - zA * zB;
    
    // The distance is acosh(lorentz_product)
    double distance;
    if(lorentz_product < 1.0) {
        lorentz_product = 1.0; // numerical safety
    }
    distance = acosh(lorentz_product);
    
    cout << fixed << setprecision(10) << distance << endl;
    
    return 0;
}
