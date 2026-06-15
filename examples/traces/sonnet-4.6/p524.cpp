#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<double> x(n);
    for(int i=0;i<n;i++) cin>>x[i];
    auto cost=[&](double d)->pair<double,double>{
        vector<double> z(n);
        for(int i=0;i<n;i++) z[i]=x[i]-i*d;
        vector<double> sz=z; sort(sz.begin(),sz.end());
        double best=1e18,ba=sz[n/2];
        for(int m=(n-1)/2;m<=n/2;m++){
            double aa=sz[m],c=0;
            for(int i=0;i<n;i++) c+=fabs(z[i]-aa);
            if(c<best){best=c;ba=aa;}
        }
        return {best,ba};
    };
    double lo=0,hi=20000;
    for(int iter=0;iter<300;iter++){
        double m1=lo+(hi-lo)/3,m2=hi-(hi-lo)/3;
        if(cost(m1).first<cost(m2).first) hi=m2; else lo=m1;
    }
    double d=(lo+hi)/2;
    auto [c,a]=cost(d);
    printf("%.4f\n",c);
    for(int i=0;i<n;i++) printf("%.10f%c",a+i*d," \n"[i==n-1]);
}
