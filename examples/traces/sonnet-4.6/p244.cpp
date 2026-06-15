#include <bits/stdc++.h>
using namespace std;

int main(){
    double h, b, m;
    // skip "Test #N" lines
    string line;
    while(getline(cin, line)){
        if(line.find("Test") != string::npos) continue;
        istringstream iss(line);
        if(iss >> h >> b >> m) break;
    }
    
    if(h > b || b > m){ cout << "NO\n"; return 0; }
    if(m*m < h*h){ cout << "NO\n"; return 0; }
    
    double p = sqrt(m*m - h*h);
    // f(a) = AD(a), search for a where AD = b
    // A = (p, h), B = (-a, 0), C = (a, 0)
    auto AD = [&](double a, double pp) -> double {
        double u = sqrt((pp+a)*(pp+a)+h*h);
        double v = sqrt((pp-a)*(pp-a)+h*h);
        double Dx = a*(u-v)/(u+v);
        return sqrt((pp-Dx)*(pp-Dx)+h*h);
    };
    
    // try p and -p
    for(int sign : {1, -1}){
        double pp = sign * p;
        double lo = 1e-9, hi = 1e9;
        double flo = AD(lo, pp), fhi = AD(hi, pp);
        if((flo - b)*(fhi - b) > 0) continue;
        for(int i = 0; i < 200; i++){
            double mid = (lo+hi)/2;
            if((AD(lo,pp)-b)*(AD(mid,pp)-b) <= 0) hi = mid;
            else lo = mid;
        }
        double a = (lo+hi)/2;
        if(fabs(AD(a,pp)-b) < 1e-6){
            cout << "YES\n";
            cout << fixed << setprecision(8) << pp << " " << h << "\n";
            cout << fixed << setprecision(8) << -a << " " << 0.0 << "\n";
            cout << fixed << setprecision(8) << a << " " << 0.0 << "\n";
            return 0;
        }
    }
    cout << "NO\n";
}
