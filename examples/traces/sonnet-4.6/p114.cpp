#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    vector<double> X(n), P(n);
    for(int i = 0; i < n; i++){
        cin >> X[i] >> P[i];
    }
    
    auto f = [&](double pos) -> double {
        double sum = 0;
        for(int i = 0; i < n; i++){
            sum += P[i] * fabs(X[i] - pos);
        }
        return sum;
    };
    
    double lo = 0, hi = 50000;
    for(int iter = 0; iter < 300; iter++){
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if(f(m1) < f(m2)){
            hi = m2;
        } else {
            lo = m1;
        }
    }
    
    double ans = (lo + hi) / 2.0;
    
    cout << fixed << setprecision(5) << ans << endl;
    
    return 0;
}
