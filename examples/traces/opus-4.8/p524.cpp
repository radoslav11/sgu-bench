#include <bits/stdc++.h>
using namespace std;

int n;
vector<double> x;

double evalCost(double d, double &a) {
    vector<double> b(n);
    for (int i = 0; i < n; i++) b[i] = x[i] - (double)i * d;
    vector<double> s = b;
    sort(s.begin(), s.end());
    a = s[n/2];
    double cost = 0;
    for (int i = 0; i < n; i++) cost += fabs(b[i] - a);
    return cost;
}

int main(){
    cin >> n;
    x.resize(n);
    for (int i = 0; i < n; i++) cin >> x[i];
    
    double lo = 0.0, hi = 40000.0;
    double atmp;
    for (int it = 0; it < 300; it++) {
        double m1 = lo + (hi - lo)/3.0;
        double m2 = hi - (hi - lo)/3.0;
        double c1 = evalCost(m1, atmp);
        double c2 = evalCost(m2, atmp);
        if (c1 < c2) hi = m2; else lo = m1;
    }
    double d = (lo + hi)/2.0;
    double a;
    double cost = evalCost(d, a);
    
    printf("%.4f\n", cost);
    for (int i = 0; i < n; i++) {
        double coord = a + (double)i * d;
        printf("%.10f", coord);
        if (i + 1 < n) printf(" ");
    }
    printf("\n");
    return 0;
}
