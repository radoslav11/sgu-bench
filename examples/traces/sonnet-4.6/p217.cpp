#include <bits/stdc++.h>
using namespace std;

int main(){
    double R1, R2;
    cin >> R1 >> R2;
    
    double lim = min(R1, R2);
    
    // Numerical integration using Simpson's rule
    int N = 1000000;
    double h = lim / N;
    double sum = 0.0;
    
    auto f = [&](double y) {
        return sqrt((R1*R1 - y*y) * (R2*R2 - y*y));
    };
    
    sum = f(0) + f(lim);
    for(int i = 1; i < N; i++){
        double y = i * h;
        sum += (i % 2 == 0 ? 2.0 : 4.0) * f(y);
    }
    sum *= h / 3.0;
    sum *= 8.0;
    
    cout << fixed << setprecision(4) << sum << endl;
    return 0;
}
