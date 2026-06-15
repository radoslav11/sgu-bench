#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main(){
    double R1, R2;
    cin >> R1 >> R2;
    
    double minR = min(R1, R2);
    
    // Numerical integration using Simpson's rule
    int n = 100000; // number of intervals
    double h = minR / n;
    
    double sum = 0.0;
    
    // Simpson's rule
    for(int i = 0; i <= n; i++){
        double y = i * h;
        double f = sqrt(R1*R1 - y*y) * sqrt(R2*R2 - y*y);
        
        if(i == 0 || i == n){
            sum += f;
        } else if(i % 2 == 1){
            sum += 4.0 * f;
        } else {
            sum += 2.0 * f;
        }
    }
    
    double integral = sum * h / 3.0;
    double volume = 8.0 * integral;
    
    cout << fixed << setprecision(4) << volume << endl;
    
    return 0;
}
