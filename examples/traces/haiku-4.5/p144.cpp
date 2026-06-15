#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

int main(){
    int X, Y;
    double Z;
    cin >> X >> Y >> Z;
    
    double T = 60.0 * (Y - X);
    double probability = (2.0 * T * Z - Z * Z) / (T * T);
    
    cout << fixed << setprecision(7) << probability << endl;
    
    return 0;
}
