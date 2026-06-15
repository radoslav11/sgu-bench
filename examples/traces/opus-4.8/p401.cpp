#include <bits/stdc++.h>
using namespace std;

int main(){
    int N; double u,t;
    cin>>N>>u>>t;
    vector<double> w(N), v(N);
    double maxv=0, W=0, minsum=0;
    for(int i=0;i<N;i++){
        cin>>w[i]>>v[i];
        maxv=max(maxv,v[i]);
        W+=w[i];
        minsum+=w[i]/u;
    }
    if(t < minsum - 1e-9){
        cout<<-1<<"\n";
        return 0;
    }
    // binary search lambda
    auto sumT=[&](double lam, vector<double>& ts)->double{
        double s=0;
        for(int i=0;i<N;i++){
            double c=lam-v[i];
            if(c<=u){ // would be infinite; clamp
                ts[i]=1e18;
                s=1e18;
            } else {
                double ti = c*w[i]/(u*sqrt(c*c-u*u));
                ts[i]=ti;
                s+=ti;
            }
        }
        return s;
    };
    double lo = u + maxv + 1e-12;
    double hi = 1e15;
    vector<double> ts(N);
    // sumT decreasing in lambda
    for(int it=0; it<300; it++){
        double mid=(lo+hi)/2;
        double s=sumT(mid,ts);
        if(s > t){ lo=mid; } else { hi=mid; }
    }
    double lam=(lo+hi)/2;
    sumT(lam, ts);
    // compute Y
    double Y=0;
    for(int i=0;i<N;i++){
        double ti=ts[i];
        // ensure ti>=w/u
        if(ti < w[i]/u) ti=w[i]/u;
        double inside = u*u*ti*ti - w[i]*w[i];
        if(inside<0) inside=0;
        Y += sqrt(inside) + v[i]*ti;
    }
    double dist = sqrt(W*W + Y*Y);
    cout<<fixed<<setprecision(10)<<dist<<"\n";
    for(int i=0;i<N;i++){
        double ti=ts[i];
        if(ti < w[i]/u) ti=w[i]/u;
        cout<<fixed<<setprecision(10)<<ti;
        if(i+1<N) cout<<" ";
    }
    cout<<"\n";
    return 0;
}
