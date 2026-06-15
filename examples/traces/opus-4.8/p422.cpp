#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; double t;
    if(!(cin>>n>>t)) return 0;
    vector<double> a(n+2);
    for(int i=1;i<=n;i++) cin>>a[i];
    vector<double> E(n+1,0.0);
    E[n]=0.0;
    for(int i=n-1;i>=0;i--){
        double f=1.0, A=0.0, B=0.0, Cns=0.0;
        double minR=1e300;
        for(int j=i+1;j<=n;j++){
            double term = f*a[j];
            A += term;
            B += term*(double)j;
            if(j!=i+1) Cns += term*E[j-1];
            double Pij = f*(1.0-a[j]);
            double Snoself = (double)(j+1)*A - B + Cns;
            double Cost = (double)(j-i) + t + Snoself + Pij*E[j];
            if(Cost<minR) minR=Cost;
            f *= (1.0-a[j]);
        }
        double denom = 1.0 - a[i+1];
        E[i] = minR/denom;
    }
    printf("%.15f\n", E[0]);
    return 0;
}
