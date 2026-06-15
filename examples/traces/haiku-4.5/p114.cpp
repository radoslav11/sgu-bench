#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    vector<double> x(n), p(n);
    
    for(int i = 0; i < n; i++){
        cin >> x[i] >> p[i];
    }
    
    auto calcDispleasure = [&](double pos) -> double {
        double sum = 0;
        for(int i = 0; i < n; i++){
            sum += p[i] * abs(pos - x[i]);
        }
        return sum;
    };
    
    double left = 0, right = 50000;
    
    for(int iter = 0; iter < 100; iter++){
        double mid1 = left + (right - left) / 3;
        double mid2 = right - (right - left) / 3;
        
        if(calcDispleasure(mid1) > calcDispleasure(mid2)){
            left = mid1;
        } else {
            right = mid2;
        }
    }
    
    cout << fixed << setprecision(5) << (left + right) / 2 << endl;
    
    return 0;
}
