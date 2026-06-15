#include <bits/stdc++.h>
using namespace std;

int main() {
    int X, Y;
    double Z;
    cin >> X >> Y >> Z;
    
    double T = (Y - X) * 60.0;
    
    double prob;
    if (Z >= T) {
        prob = 1.0;
    } else {
        double ratio = (T - Z) / T;
        prob = 1.0 - ratio * ratio;
    }
    
    printf("%.7f\n", prob);
    return 0;
}
